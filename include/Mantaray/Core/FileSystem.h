#include <string>

namespace MR{
class FileSystem {
    public:
        static std::string getWorkingDirectory();
        static bool readFile(std::string path, std::string& content, bool isAbsolute = false);
};
}
