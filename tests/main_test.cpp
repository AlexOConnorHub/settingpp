 //
// Created by alex on 5/7/22.
//

#include "settingpp.hpp"

int main() {
    auto settings = new settingpp::settings("PiEar");
    settings->set("test", "test");
    settings->set("test2", "test2");
    settings->set("test3", "test3");
    settings->get<std::string>("test");
    delete settings;
    return 0;
}