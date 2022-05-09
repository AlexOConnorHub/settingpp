#include "all_test.h"
#include "settingpp.hpp"
#include <iostream>
#include <string>

bool load_write_file_test(bool verbose) {
    bool success = true;
    auto* settings = new settingpp::settings("write_file", "first_test.json");
    settings->set("test", "test");
    settings->set("test2", "test2");
    delete settings;
    settings = new settingpp::settings("write_file", "first_test.json");
    if (settings->get<std::string>("test") != "test") {
        success = false;
        if (verbose) {
            std::cerr << "test failed" << std::endl;
        }
    }
    if (settings->get<std::string>("test2") != "test2") {
        success = false;
        if (verbose) {
            std::cerr << "test2 failed" << std::endl;
        }
    }
    delete settings;
    settings = new settingpp::settings("write_file");
    settings->set("string", "test");
    settings->set("int", 1);
    settings->set("double", 1.1);
    settings->set("bool", true);
    delete settings;
    settings = new settingpp::settings("write_file");
    if (settings->get<std::string>("string") != "test") {
        success = false;
        if (verbose) {
            std::cerr << "string failed" << std::endl;
        }
    }
    if (settings->get<int>("int") != 1) {
        success = false;
        if (verbose) {
            std::cerr << "int failed" << std::endl;
        }
    }
    if (settings->get<double>("double") != 1.1) {
        success = false;
        if (verbose) {
            std::cerr << "double failed" << std::endl;
        }
    }
    if (!settings->get<bool>("bool")) {
        success = false;
        if (verbose) {
            std::cerr << "bool failed" << std::endl;
        }
    }
    delete settings;
    return success;
}