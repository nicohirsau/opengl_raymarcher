#include <string>

#include "Mantaray/Core/Vector.h"

namespace MR {
class Texture;

class Image {
    friend class Texture;

    public:
        Image(std::string pathToImage);
        Image(unsigned char* imageData, int numPixels);
        ~Image();

        void loadFromFile(std::string pathToImage);
        void unloadData();
        Texture uploadImageData();

        int getWidth();
        int getHeight();

    private:
        unsigned char* m_ImageData;
        Vector2i m_Size;
        int m_NrChannels;
};
}