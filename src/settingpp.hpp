//
// Created by alex on 5/7/22.
//

#ifndef SETTINGPP_SETTINGPP_HPP
#define SETTINGPP_SETTINGPP_HPP

#include <string>
#include "json.hpp"
#include <mutex>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <csignal>
#include <pwd.h>
#include <iostream>

// Preprocessor if Windows
#if defined(_WIN32) || defined(WIN32)
    #define SETTINGPP_WINDOWS
    #define DS '\\'
    #define HOME_ENV "APPDATA"
    #define SAVE_PATH(home) (home) + DS + "Roaming" + DS
#elif defined(__APPLE__)
    #define SETTINGPP_MACOS
    #define DS '/'
    #define HOME_ENV "HOME"
    #define SAVE_PATH(home) (home) + DS + "Library" + DS + "Application Support" + DS
#elif defined(__linux__)
    #define SETTINGPP_LINUX
    #define DS '/'
    #define HOME_ENV "HOME"
    #define SAVE_PATH(home) (home) + ".config" + DS
#endif

// Preprocessor Macro Fuction
namespace settingpp {

    class settings {
    private:
        static std::string get_home_env() {
            std::string home_env = (std::getenv(HOME_ENV) == nullptr) ? "" : std::getenv(HOME_ENV);
            if (home_env.empty()) {
                #ifndef SETTINGPP_WINDOWS
                    struct passwd *pw = getpwuid(getuid());
                    home_env = std::string(pw->pw_dir);
                    if (home_env.empty()) {
                        throw std::runtime_error("Failed to get home directory");
                    }
                #else
                    throw std::runtime_error("Failed to get home directory");
                #endif
            }
            return home_env;
        }
        std::string save_path = get_home_env();

    public:
        #define PROJECT_SETTING_PATH(project, file) (SAVE_PATH(save_path) + (project) + DS + (file))
        explicit settings(const std::string& project, const std::string& path = ""):
                config_file(PROJECT_SETTING_PATH(project, (path.empty() ? project + ".json" : path))),
                config_directory(config_file.substr(0, config_file.find_last_of(DS))),
                config_file_name(config_file.substr(config_file.find_last_of(DS) + 1)),
                settings_json(nlohmann::json::parse(load())) {
        }
        ~settings() {
            save();
        }

        std::mutex mtx;
        std::string config_file;
        std::string config_directory;
        std::string config_file_name;
        nlohmann::json settings_json;

        // Load settings from file
        std::string load() {
            // Check if directory exists
            if (!std::filesystem::exists(config_directory)) {
                std::filesystem::create_directory(config_directory);
            }
            if (!std::filesystem::exists(config_file)) {
                mtx.lock();
                std::ofstream file(config_file);
                file.close();
                mtx.unlock();
                return "{}";
            }
            mtx.lock();
            std::ifstream file(config_file);
            if (file.is_open()) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                file.close();
                mtx.unlock();
                return buffer.str();
            } else {
                std::cout << "File not found: " << config_file << std::endl;
            }
        }

        // Save settings to file
        void save() {
            mtx.lock();
            std::ofstream file(config_file);
            if (file.is_open()) {
                file << settings_json;
                file.close();
                mtx.unlock();
            } else {
                std::cout << "File not found: " << config_file << std::endl;
            }
        }

        // Get setting as template parameter
        template <typename T>
        T get(std::string key) {
            return settings_json[std::move(key)].get<T>();
        }

        // Set setting
        template <typename T>
        void set(std::string key, T value) {
            settings_json[std::move(key)] = value;
        }
    };
}
#endif //SETTINGPP_SETTINGPP_HPP
