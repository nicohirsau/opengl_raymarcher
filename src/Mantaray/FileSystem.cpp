#include <fstream>
#include <streambuf>

#include <Windows.h>

#include "Mantaray/Core/FileSystem.h"
#include "Mantaray/Core/Logger.h"

using namespace MR;

std::string FileSystem::getWorkingDirectory() {
    // Windows specific way of checking if file exists
    char result[MAX_PATH];
    GetModuleFileName(NULL, result, MAX_PATH);
    std::string wd = "";
    for (int i = 0; i < MAX_PATH; i++){
        wd += result[i];
        if (result[i] == '\0'){
            break;
        }
    }
    return wd.substr(0, wd.find_last_of("\\/") + 1);
}

bool FileSystem::readFile(std::string path, std::string& content, bool absolutePath) {
    if (!absolutePath){
        path = FileSystem::getWorkingDirectory() + path;
    }

    std::ifstream t(path.c_str());
    if (!t){
        Logger::Log("FileSystem", "Could not open file: " + path, Logger::LOG_ERROR);
        return false;
    }
    content.clear();
    content = std::string((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
    return true;
}
