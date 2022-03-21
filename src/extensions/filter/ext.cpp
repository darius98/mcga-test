#include "ext.hpp"

#include "test_description.hpp"

#include <charconv>
#include <regex>

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
    return std::regex_search(getTestFullDescription(test), filter) == exclude;
}

// Returns whether the test should be skipped.
static bool matchLocationFilter(const std::tuple<std::regex, int, int>& filter,
                                bool exclude,
                                const mcga::test::Test& test) {
    const auto& [file, line, col] = filter;
    bool matches = std::regex_search(test.getContext().fileName.c_str(), file);
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
  const std::vector<std::string>& rawValue,
  T (*callback)(std::string_view),
  bool (*match)(const T&, bool, const mcga::test::Test&),
  bool exclude,
  std::vector<std::function<bool(const mcga::test::Test&)>>& out) {
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

FilterExtension::FilterExtension(
  const std::vector<std::string>& descriptionFilter,
  const std::vector<std::string>& descriptionExclude,
  const std::vector<std::string>& locationFilter,
  const std::vector<std::string>& locationExclude) {
    parseFilterArray(descriptionFilter,
                     parseDescriptionFilter,
                     matchDescriptionFilter,
                     false,
                     filters);
    parseFilterArray(descriptionExclude,
                     parseDescriptionFilter,
                     matchDescriptionFilter,
                     true,
                     filters);
    parseFilterArray(
      locationFilter, parseLocationFilter, matchLocationFilter, false, filters);
    parseFilterArray(
      locationExclude, parseLocationFilter, matchLocationFilter, true, filters);
}

void FilterExtension::beforeTestExecution(
  const Test& test, std::optional<Test::ExecutionInfo>& info) {
    if (shouldSkipTest(test)) {
        info = Test::ExecutionInfo{
          .status = Test::ExecutionInfo::SKIPPED,
          .message = "Filtered out by command line arguments.",
          .context = std::nullopt,
          .timeTicks = -1.0,
        };
    }
}

bool FilterExtension::shouldSkipTest(const Test& t) const {
    return std::any_of(
      filters.begin(), filters.end(), [&t](const auto& filter) {
          return filter(t);
      });
}

}  // namespace mcga::test
