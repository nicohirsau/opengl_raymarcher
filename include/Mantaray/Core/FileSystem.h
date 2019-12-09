#include <string>

namespace MR{
class FileSystem {
    public:
        static std::string getWorkingDirectory();
        static bool readFile(std::string path, std::string& content, bool absolutePath = false);
        static bool loadImage(std::string path, unsigned char*& data, int& width, int& height, int& nrChannels, bool absolutePath = false);
};
}
