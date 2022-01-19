#include "ext.hpp"

#include <charconv>

static bool needs_regex_escape(char ch) {
    return ch == '.' || ch == '[' || ch == '\\' || ch == '^' || ch == '$';
}

static std::regex parseDescriptionFilter(std::string_view filter) {
    if (filter.empty()) {
        return std::regex(".*");
    }
    std::string regex_data;
    regex_data.reserve(
      filter.size() + std::count(filter.begin(), filter.end(), '*')
      + std::count_if(filter.begin(), filter.end(), needs_regex_escape)
      + 2 * (!filter.starts_with('*')) + 2 * (!filter.ends_with('*')));
    if (!filter.starts_with('*')) {
        regex_data += ".*";
    }
    for (char ch: filter) {
        if (ch == '*') {
            regex_data += ".*";
        } else {
            if (needs_regex_escape(ch)) {
                regex_data += '\\';
            }
            regex_data += ch;
        }
    }
    if (!filter.ends_with('*')) {
        regex_data += ".*";
    }
    return std::regex{
      regex_data, std::regex_constants::optimize | std::regex_constants::basic};
}

static int parseNumberOrStar(std::string_view filter) {
    if (filter.empty() || (filter.size() == 1 && filter[0] == '*')) {
        return -1;
    }
    int val;
    const auto status = std::from_chars(filter.begin(), filter.end(), val);
    if (status.ec != std::errc{}) {
        throw std::invalid_argument(
          "Invalid number format provided to --filter-loc or --exclude-loc");
    }
    return val;
}

static std::tuple<std::regex, int, int>
  parseLocationFilter(std::string_view filter) {
    const auto last_colon = filter.find_last_of(':');
    if (last_colon == std::string::npos) {
        return {parseDescriptionFilter(filter), -1, -1};
    }
    const auto second_last_colon = filter.find_last_of(':', last_colon - 1);
    if (last_colon == 0 || second_last_colon == std::string::npos) {
        return {
          parseDescriptionFilter(filter.substr(0, last_colon)),
          parseNumberOrStar(filter.substr(last_colon + 1)),
          -1,
        };
    }
    return {
      parseDescriptionFilter(filter.substr(0, second_last_colon)),
      parseNumberOrStar(filter.substr(second_last_colon + 1,
                                      last_colon - second_last_colon - 1)),
      parseNumberOrStar(filter.substr(second_last_colon + 1)),
    };
}

// Returns whether the test should be skipped.
static bool matchDescriptionFilter(const std::regex& filter,
                                   bool exclude,
                                   const mcga::test::Test& test) {
    return std::regex_search(test.getFullDescription(), filter) == exclude;
}

// Returns whether the test should be skipped.
static bool matchLocationFilter(const std::tuple<std::regex, int, int>& filter,
                                bool exclude,
                                const mcga::test::Test& test) {
    const auto& [file, line, col] = filter;
    bool matches = std::regex_search(test.getContext().fileName, file);
    if (line != -1) {
        matches &= (line == test.getContext().line);
    }
    if (col != -1) {
        matches &= (col == test.getContext().column);
    }
    return matches == exclude;
}

template<class T>
static void parseFilterArray(
  const mcga::cli::ListArgument<>& arg,
  T (*callback)(std::string_view),
  bool (*match)(const T&, bool, const mcga::test::Test&),
  bool exclude,
  std::vector<std::function<bool(const mcga::test::Test&)>>& out) {
    const auto rawValue = arg->get_value();
    out.reserve(out.size() + rawValue.size());
    std::transform(rawValue.begin(),
                   rawValue.end(),
                   std::back_inserter(out),
                   [callback, match, exclude](std::string_view raw) {
                       auto filter = callback(raw);
                       return [filter = std::move(filter), match, exclude](
                                const mcga::test::Test& t) {
                           return match(filter, exclude, t);
                       };
                   });
}

namespace mcga::test {

void FilterExtension::registerCommandLineArgs(cli::Parser* parser) {
    descriptionFilterArgument = parser->add_list_argument(
      cli::ListArgumentSpec{"filter"}
        .set_short_name("f")
        .set_help_group("Filtering")
        .set_description(
          "Filter tests by their full description "
          "(e.g. \"TestCaseName::group description::...::test description\")"
          "You can use the '*' character to match any number of characters. A "
          "test's description must contain a match, so writing "
          "\"--filter=*foo*\" is equivalent to writing \"--filter=foo\". If "
          "multiple filters are provided, at least one of them must match.")
        .set_default_value({}));
    descriptionExcludeArgument = parser->add_list_argument(
      cli::ListArgumentSpec{"exclude"}
        .set_short_name("e")
        .set_help_group("Filtering")
        .set_description("Same as --filter, except matching tests are skipped "
                         "instead of being executed.")
        .set_default_value({}));
    locationFilterArgument = parser->add_list_argument(
      cli::ListArgumentSpec{"filter-loc"}
        .set_help_group("Filtering")
        .set_description(
          "Filter tests by source code location. "
          "Expects the format --filter-loc=FILE or --filter-loc=FILE:LINE or "
          "--filter-loc=FILE:LINE:COLUMN, where the FILE will automatically "
          "match the suffix of the file. You can use the '*' character "
          "within "
          "the FILE portion to match any number of characters. You can also "
          "use the '*' character instead of LINE or COLUMN (e.g. "
          "--filter-loc=my_tests.cpp is equivalent to "
          "--filter-loc=*my_tests.cpp:*:*). If multiple filters are provided, "
          "at least one of them must match.")
        .set_default_value({}));
    locationExcludeArgument = parser->add_list_argument(
      cli::ListArgumentSpec{"exclude-loc"}
        .set_help_group("Filtering")
        .set_description(
          "Same as --filter-loc, except matching tests are "
          "skipped instead of being executed. Using '*' for LINE or COLUMN, or "
          "not including the :LINE or :LINE:COLUMN section means the "
          "line/column is not considered when filtering.")
        .set_default_value({}));
}

void FilterExtension::init(ExtensionApi* api) {
    if (descriptionFilterArgument->appeared()
        || descriptionExcludeArgument->appeared()
        || locationFilterArgument->appeared()
        || locationExcludeArgument->appeared()) {

        parseFilterArray(descriptionFilterArgument,
                         parseDescriptionFilter,
                         matchDescriptionFilter,
                         false,
                         filters);
        parseFilterArray(descriptionExcludeArgument,
                         parseDescriptionFilter,
                         matchDescriptionFilter,
                         true,
                         filters);
        parseFilterArray(locationFilterArgument,
                         parseLocationFilter,
                         matchLocationFilter,
                         false,
                         filters);
        parseFilterArray(locationExcludeArgument,
                         parseLocationFilter,
                         matchLocationFilter,
                         true,
                         filters);

        api->addHook<ExtensionApi::BEFORE_TEST_EXECUTION>(
          [this](const Test& test, std::optional<Test::ExecutionInfo>& info) {
              if (shouldSkipTest(test)) {
                  info = Test::ExecutionInfo{
                    .status = Test::ExecutionInfo::SKIPPED,
                    .timeTicks = -1.0,
                    .message = "Filtered out by command line arguments.",
                    .context = std::nullopt,
                  };
              }
          });
    }
}

bool FilterExtension::shouldSkipTest(const Test& t) const {
    return std::any_of(
      filters.begin(), filters.end(), [&t](const auto& filter) {
          return filter(t);
      });
}

}  // namespace mcga::test
