#include "all_test.h"
#include <iostream>
#include <vector>
#include "settingpp.hpp"

bool data_type_test(bool verbose) {
    bool final = true;
    auto s = settingpp::settings("data_type_test");
    s.set("int", 1);
    s.set("float", 1.0);
    s.set("double", 1.0);
    s.set("string", "test");
    s.set("bool", true);
    s.set("null", nullptr);
//    s.set("array", {1, 2, 3});
//    s.set("object", {{"a", 1}, {"b", 2}});

    if (s.get<int>("int") != 1) {
        if (verbose) {
            std::cerr << "int: " << s.get<int>("int") << " != 1" << std::endl;
        }
        final = false;
    }
    if (s.get<float>("float") != 1.0) {
        if (verbose) {
            std::cerr << "float: " << s.get<float>("float") << " != 1.0" << std::endl;
        }
        final = false;
    }
    if (s.get<double>("double") != 1.0) {
        if (verbose) {
            std::cerr << "double: " << s.get<double>("double") << " != 1.0" << std::endl;
        }
        final = false;
    }
    if (s.get<std::string>("string") != "test") {
        if (verbose) {
            std::cerr << "string: " << s.get<std::string>("string") << " != test" << std::endl;
        }
        final = false;
    }
    if (!s.get<bool>("bool")) {
        if (verbose) {
            std::cerr << "bool: " << s.get<bool>("bool") << " != true" << std::endl;
        }
        final = false;
    }
    try {
        s.get<std::nullptr_t>("null");
    } catch (...) {
        if (verbose) {
            std::cerr << "data_type: null test failed" << std::endl;
        }
        final = false;
    }
    return final;
}