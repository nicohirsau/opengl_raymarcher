#include <glad/glad.h>

#include "Mantaray/Graphics/Texture.h"
#include "Mantaray/Graphics/Image.h"

using namespace MR;

Texture::Texture() {}

Texture::Texture(std::string pathToTexture) {
    Image i = Image(pathToTexture);
    uploadTextureData(i.m_ImageData, i.getWidth(), i.getHeight(), i.m_NrChannels);
}

Texture::Texture(unsigned char* textureData, int width, int height, int nrChannels) {
    uploadTextureData(textureData, width, height, nrChannels);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureID);
}

void Texture::setFromImage(Image &image) {
    uploadTextureData(image.m_ImageData, image.getWidth(), image.getHeight(), image.m_NrChannels);
}

void Texture::uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels) {
    m_Size = Vector2i(width, height);
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 0x1907 + nrChannels - 3, GL_UNSIGNED_BYTE, textureData);   
}

int Texture::getWidth() {
    return m_Size.x;
}

int Texture::getHeight() {
    return m_Size.y;
}

unsigned int Texture::getTextureID() {
    return m_TextureID;
}
