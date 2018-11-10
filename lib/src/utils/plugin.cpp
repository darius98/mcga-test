#include "plugin.hpp"

using namespace std;


namespace kktest {

map<string, vector<Plugin*>> Plugin::plugins;

Plugin::Plugin(const string& groupName) {
    if (!plugins.count(groupName)) {
        plugins[groupName] = vector<Plugin*>();
    }
    plugins[groupName].push_back(this);
}

Plugin::~Plugin() = default;

bool Plugin::isEnabled() const {
    return true;
}

void Plugin::install() {
}

void Plugin::uninstall() {
}

Pluginable::Pluginable(const string& _pluginGroupName): pluginGroupName(_pluginGroupName) {}

Pluginable::~Pluginable() = default;

void Pluginable::installPlugins() {
    for (Plugin* plugin: Plugin::plugins[pluginGroupName]) {
        if (plugin->isEnabled()) {
            plugin->install();
        }
    }
}

void Pluginable::uninstallPlugins() {
    for (Plugin* plugin: Plugin::plugins[pluginGroupName]) {
        if (plugin->isEnabled()) {
            plugin->uninstall();
        }
    }
}

}
