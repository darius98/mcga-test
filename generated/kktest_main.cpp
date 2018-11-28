// TODO(darius98): This should be auto-generated from a list of extensions.

#include <EasyFlags.hpp>

#include <kktest_impl/main.hpp>

#include <kktest_ext/feedback_impl/ext.hpp>

int main(int argc, char** argv) {

    kktest::feedback::FeedbackExtensionManager feedbackExtensionManager;

    easyflags::ParseEasyFlags(argc, argv);
    std::vector<kktest::Plugin*> plugins;
    std::function<void(kktest::Plugin*)> registerPlugin = [&plugins](kktest::Plugin* plugin) {
        plugins.push_back(plugin);
    };

    feedbackExtensionManager.init(registerPlugin);

    int ret = kktest::main(plugins);

    feedbackExtensionManager.destroy();

    return ret;
}
