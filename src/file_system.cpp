#include "file_system.hpp"
#include <boost/filesystem.hpp>
#include <fstream>

FileSystem::FileSystem() {}

bool FileSystem::createFile(const std::string& path) {
    std::ofstream file(path);
    return file.good();
}

bool FileSystem::deleteFile(const std::string& path) {
    return boost::filesystem::remove(path);
}

bool FileSystem::writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file) return false;
    file << content;
    return true;
}

std::string FileSystem::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) return "";
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

std::vector<std::string> FileSystem::listFiles(const std::string& directory) {
    std::vector<std::string> files;
    for (auto& entry : boost::filesystem::directory_iterator(directory)) {
        files.push_back(entry.path().string());
    }
    return files;
}
