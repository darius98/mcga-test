#include "module.hpp"

using namespace std;


namespace kktest {

bool Module::isEnabled() const {
    return true;
}

void Module::install() {
}

void Module::uninstall() {
}

Pluginable::Pluginable() = default;

Pluginable::~Pluginable() {
    for (Module* module: modules) {
        delete module;
    }
}

void Pluginable::addModules(const vector<Module*>& newModules) {
    modules.insert(modules.end(), newModules.begin(), newModules.end());
}

void Pluginable::installModules() {
    for (Module* module: modules) {
        if (module->isEnabled()) {
            module->install();
        }
    }
}

void Pluginable::uninstallModules() {
    for (Module* module: modules) {
        if (module->isEnabled()) {
            module->install();
        }
    }
}

}
