#ifndef SETTINGPP_SETTINGPP_HPP
#define SETTINGPP_SETTINGPP_HPP
#include <string>
#include "json.hpp"
#include <mutex>
#include <fstream>
#include <filesystem>
#include <csignal>
#include <pwd.h>
#include <regex>

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
namespace settingpp {
    /// @brief settingpp::Settingpp is a class that provides a simple way to store and retrieve settings.
    class settings {
    private:
        /// @brief Validates a filename. Only allows alphanumeric characters,
        /// dashes, and underscores, and will replace spaces with underscores.
        /// File must also be less than 100 characters.
        /// @param filename The filename to validate.
        /// @return The validated filename.
        static std::string validate_name(std::string name) {
            std::replace(name.begin(), name.end(), ' ', '_');
            if (name.empty()) {
                throw std::invalid_argument("name is empty");
            } else if (name.length() > 100) {
                throw std::invalid_argument("name is too long");
            } else if (std::regex_match(name, std::regex("[^a-zA-Z0-9_-]"))) {
                throw std::invalid_argument("name contains invalid character(s)");
            }
            return name;
        }
        /// @brief Gets the home directory.
        /// @return The home directory.
        static std::string get_home_env() {
            std::string home_env = (std::getenv(HOME_ENV) == nullptr) ? "" : std::getenv(HOME_ENV);
            if (home_env.empty()) {
                #if defined(SETTINGPP_LINUX) || defined(SETTINGPP_MACOS)
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
        /// @brief save_path is the path to the directory where settings are saved.
        std::string save_path = get_home_env();
    public:
        #define PROJECT_SETTING_PATH(project, file) (SAVE_PATH(save_path) + (project) + DS + (file))
        /// @brief Creates a settings file with the given name, in the default location for the current platform.
        /// @param project The project name
        /// @param file The file name
        explicit settings(const std::string& project, const std::string& path = ""):
                config_file(PROJECT_SETTING_PATH(validate_name(project), validate_name(path.empty() ? project + ".json" : path))),
                config_directory(config_file.substr(0, config_file.find_last_of(DS))),
                config_file_name(config_file.substr(config_file.find_last_of(DS) + 1)),
                settings_json(load()) {}
        /// @brief Saves the settings to the file upon destruction.
        ~settings() {
            save();
            delete settings_json;
        }
        /// @brief Mutex for the settings file.
        std::mutex file_mtx;
        /// @brief Mutex for the settings json.
        std::mutex json_mtx;
        /// @brief The full path of the settings file.
        const std::string config_file;
        /// @brief The directory of the settings file.
        const std::string config_directory;
        /// @brief The name of the settings file.
        const std::string config_file_name;
        /// @brief The settings in JSON format.
        nlohmann::json *settings_json;
        /// @brief Loads the settings from the file, called up upon construction.
        nlohmann::json* load() {
            // Check if directory exists
            if (!std::filesystem::exists(config_directory)) {
                std::filesystem::create_directory(config_directory);
            }
            if (!std::filesystem::exists(config_file)) {
                file_mtx.lock();
                std::ofstream file(config_file);
                file << "{}";
                file.close();
                file_mtx.unlock();
                return new nlohmann::json();
            }
            file_mtx.lock();
            std::ifstream file(config_file);
            std::stringstream buffer;
            if (file.is_open()) {
                buffer << file.rdbuf();
                file.close();
                file_mtx.unlock();
            } else {
                throw std::runtime_error("File not found: " + config_file);
            }
            try {
                nlohmann::basic_json final = nlohmann::json::parse(buffer.str());
                return new nlohmann::json(final);
            } catch (const std::exception& e) {
                throw std::runtime_error("Failed to parse settings file: " + config_file);
            }
        }
        /// @brief Saves the settings to the file, called upon destruction.
        void save() {
            file_mtx.lock();
            std::ofstream file(config_file);
            if (file.is_open()) {
                json_mtx.lock();
                file << settings_json->dump();
                json_mtx.unlock();
                file.close();
                file_mtx.unlock();
            } else {
                throw std::runtime_error("File not found: " + config_file);
            }
        }
        /// @brief Gets the value of the given key. Must specify type.
        /// @param key The key to get the value of
        /// @return The value of the key
        template <typename T>
        T get(std::string key) {
            try {
                json_mtx.lock();
                T final =  (*settings_json)[std::move(key)].get<T>();
                json_mtx.unlock();
                return final;
            } catch (const std::exception& e) {
                throw std::runtime_error("Key not found: " + key);
            }
        }
        /// @brief Sets the value of the given key. Must specify type.
        /// @param key The key to set the value of
        /// @param value The value to set
        template <typename T>
        void set(std::string key, T value) {
            try {
                json_mtx.lock();
                (*settings_json)[key] = value;
                json_mtx.unlock();
            } catch (const std::exception& e) {
                throw std::runtime_error(std::string("Failed to set value: " + key + " " + e.what()));
            }
        }
    };
}
#endif //SETTINGPP_SETTINGPP_HPP
