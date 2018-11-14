#include "plugin.hpp"

#include <iostream>
#include <cstring>

using namespace std;


namespace kktest {

Plugin** Plugin::plugins = nullptr;
int Plugin::numPlugins = 0;
int Plugin::pluginsCapacity = 0;

Plugin::Plugin() {
    if (numPlugins == 0) {
        plugins = (Plugin**)malloc(sizeof(Plugin*) * 10);
        pluginsCapacity = 10;
    }
    if (pluginsCapacity == numPlugins) {
        auto newPluginsArray = (Plugin**)malloc(sizeof(Plugin*) * (2 * pluginsCapacity));
        memcpy(newPluginsArray, plugins, numPlugins * sizeof(Plugin*));
        free(plugins);
        plugins = newPluginsArray;
    }
    plugins[numPlugins++] = this;
}

Plugin::~Plugin() = default;

bool Plugin::isEnabled() const {
    return true;
}

void Plugin::install() {
}

void Plugin::uninstall() {
}

Pluginable::~Pluginable() = default;

void Pluginable::installPlugins() {
    for (int i = 0; i < Plugin::numPlugins; ++ i) {
        auto plugin = Plugin::plugins[i];
        if (plugin->isEnabled()) {
            plugin->install();
        }
    }
}

void Pluginable::uninstallPlugins() {
    for (int i = 0; i < Plugin::numPlugins; ++ i) {
        Plugin* plugin = Plugin::plugins[i];
        if (plugin->isEnabled()) {
            plugin->uninstall();
        }
    }
}

}
