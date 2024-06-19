#pragma once

#include <string>
#include <vector>

class FileSystem {
public:
    FileSystem();
    bool createFile(const std::string& path);
    bool deleteFile(const std::string& path);
    bool writeFile(const std::string& path, const std::string& content);
    std::string readFile(const std::string& path);
    std::vector<std::string> listFiles(const std::string& directory);
};