#include <glad/glad.h>

#include "Mantaray/Graphics/Texture.h"
#include "Mantaray/Graphics/Image.h"

using namespace MR;

Texture::Texture(std::string pathToTexture) {
    m_Image = new Image(pathToTexture);
    uploadTextureData();
}

Texture::Texture(unsigned char* textureData, int width, int height, int nrChannels) {
    m_Image = new Image(textureData, width, height, nrChannels);
    uploadTextureData();
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureID);
    delete m_Image;
}

void Texture::uploadTextureData() {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Image->getWidth(), m_Image->getHeight(), 0, 0x1907 + m_Image->m_NrChannels - 3, GL_UNSIGNED_BYTE, m_Image->m_ImageData);   
}

unsigned int Texture::getTextureID() {
    return m_TextureID;
}