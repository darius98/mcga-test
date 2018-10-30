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

Pluginable::Pluginable(const vector<Plugin*>& _plugins): plugins(_plugins) {}

Pluginable::~Pluginable() {
    for (Plugin* plugin: plugins) {
        if (plugin->isEnabled()) {
            plugin->uninstall();
        }
        delete plugin;
    }
}

void Pluginable::installPlugins() {
    for (Plugin* plugin: plugins) {
        if (plugin->isEnabled()) {
            plugin->install();
        }
    }
}

}
