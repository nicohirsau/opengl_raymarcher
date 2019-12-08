#include <algorithm>

#include "Mantaray/Graphics/Image.h"
#include "Mantaray/Graphics/Texture.h"
#include "Mantaray/Core/FileSystem.h"

using namespace MR;

Image::Image(std::string pathToImage) {
    loadFromFile(pathToImage);
}

Image::Image(unsigned char* imageData, int numPixels) {
    std::copy(imageData, imageData + numPixels, m_ImageData);
}

Image::~Image() {
    unloadData();
}

void Image::loadFromFile(std::string pathToImage) {
    FileSystem::loadImage(pathToImage, m_ImageData, m_Size.x, m_Size.y, m_NrChannels);
}

void Image::unloadData() {
    FileSystem::unloadImage(m_ImageData);
}

Texture Image::uploadImageData() {
    return Texture(m_ImageData, m_Size.x * m_Size.y);
}

int Image::getWidth() {
    return m_Size.x;
}

int Image::getHeight() {
    return m_Size.y;
}
