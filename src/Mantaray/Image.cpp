#include <algorithm>
#include <cstdlib>

#include "Mantaray/Graphics/Image.h"
#include "Mantaray/Graphics/Texture.h"
#include "Mantaray/Core/FileSystem.h"

using namespace MR;

Image::Image() {}

Image::Image(std::string pathToImage) {
    loadFromFile(pathToImage);
}

Image::Image(unsigned char* imageData, int width, int height, int nrChannels) {
    std::copy(imageData, imageData + width * height, m_ImageData);
    m_Size = Vector2i(width, height);
    m_NrChannels = nrChannels;
}

Image::~Image() {
    unloadData();
}

void Image::loadFromFile(std::string pathToImage) {
    FileSystem::loadImage(pathToImage, m_ImageData, m_Size.x, m_Size.y, m_NrChannels);
}

void Image::unloadData() {
    free(m_ImageData);
    m_ImageData = nullptr;
    m_Size = Vector2i(0, 0);
    m_NrChannels = 0;
}

int Image::getWidth() {
    return m_Size.x;
}

int Image::getHeight() {
    return m_Size.y;
}
