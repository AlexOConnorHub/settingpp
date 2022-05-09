#include "all_test.h"
#include "settingpp.hpp"
#include <iostream>

bool edge_case_test(bool verbose) {
    bool final = true;
    settingpp::settings s("edge_case_test");
    s.set("true", true);
    if (!s.get<bool>("true")) {
        if (verbose) {
            std::cout << "EdgeCaseTest: string \"true\" as name failed" << std::endl;
        }
        final = false;
    }

    return final;
}