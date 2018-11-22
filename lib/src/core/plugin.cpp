#include "plugin.hpp"

#include <cstring>

using namespace std;


namespace kktest {

vector<Plugin*>* Plugin::plugins = nullptr;

Plugin::Plugin() {
    if (plugins == nullptr) {
        plugins = new vector<Plugin*>();
    }
    plugins->push_back(this);
}

void Plugin::clean() {
    delete plugins;
}

}
