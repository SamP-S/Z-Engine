#include "common/util.hpp"

// std lib
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ostream>

namespace util {

    std::string loadTextFile(const std::string& _path) {
        // open file
        std::filesystem::path fullPath = std::filesystem::absolute(_path);
        std::ifstream file(fullPath, std::ios::in);
        // check file opened
        if (!file.is_open()) {
            std::cerr << "[Load] Failed to open text file: " << fullPath << std::endl;
            return "";
        }
        // extract data
        std::ostringstream oss;
        oss << file.rdbuf();
        std::string content = oss.str();
        std::cout << "[Load] Loaded text file: (" << content.size() << " bytes)" << fullPath << std::endl;
        return content;
    }

} // namespace util