#include <string>

namespace MR {
class Image;

class Texture {
    public:
        Texture(std::string pathToTexture);
        Texture(unsigned char* textureData, int width, int height, int nrChannels);
        ~Texture();

        unsigned int getTextureID();

    private:
        void uploadTextureData();

    private:
        unsigned int m_TextureID;
        Image* m_Image;
};
}