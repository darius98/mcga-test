#include "plugin.hpp"

using namespace std;


namespace kktest {

bool Plugin::isEnabled() const {
    return true;
}

void Plugin::install() {
}

void Plugin::uninstall() {
}

Pluginable::Pluginable() = default;

Pluginable::~Pluginable() {
    for (Plugin* plugin: plugins) {
        if (plugin->isEnabled()) {
            plugin->uninstall();
        }
        delete plugin;
    }
}

void Pluginable::addPlugins(const vector<Plugin*>& newPlugins) {
    plugins.insert(plugins.end(), newPlugins.begin(), newPlugins.end());
}

void Pluginable::installPlugins() {
    for (Plugin* plugin: plugins) {
        if (plugin->isEnabled()) {
            plugin->install();
        }
    }
}

}
