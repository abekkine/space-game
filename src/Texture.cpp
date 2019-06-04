#include "Texture.h"
#include "GameException.h"

#include <GLFW/glfw3.h>

Texture::Texture(const std::string & file_name)
: width_(0.0)
, height_(0.0)
, raw_width_(0)
, raw_height_(0)
, file_name_(file_name)
, bytes_per_pixel_(0)
, texture_format_(0)
, texture_id_(0)
, pixels_(0) {
    SDL_Surface * image = LoadFile();
    if (image != NULL) {
        GenerateTexture();
        Normalize();
        SDL_FreeSurface(image);
    }
}
Texture::~Texture() {
    glDeleteTextures(1, &texture_id_);
}
void Texture::Render() {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2d(0.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex2d(width_, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex2d(width_, height_);
        glTexCoord2f(0.0, 1.0); glVertex2d(0, height_);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

SDL_Surface * Texture::LoadFile() {
    SDL_Surface * image = IMG_Load(file_name_.c_str());
    if (image != NULL) {
        raw_width_ = image->w;
        raw_height_ = image->h;
        texture_format_ = GL_RGBA;
        bytes_per_pixel_ = image->format->BytesPerPixel;
        if (bytes_per_pixel_ == 4) {
            if (image->format->Rmask == 0xff) {
                texture_format_ = GL_RGBA;
            } else {
                texture_format_ = GL_BGRA;
            }
        } else if (bytes_per_pixel_ == 3) {
            if (image->format->Rmask == 0xff) {
                texture_format_ = GL_RGB;
            } else {
                texture_format_ = GL_BGR;
            }
        }

        pixels_ = image->pixels;
    } else {
        std::string msg = "Unable to find texture file : " + file_name_;
        throw GameException(GameException::eTextureNotFound, msg);
    }

    return image;
}
void Texture::GenerateTexture() {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        bytes_per_pixel_,
        raw_width_, raw_height_,
        0,
        texture_format_,
        GL_UNSIGNED_BYTE,
        pixels_);
}
void Texture::Normalize() {
    if (raw_width_ > raw_height_) {
        width_ = 1.0;
        height_ = static_cast<double>(raw_height_) / raw_width_;
    } else {
        height_ = 1.0;
        width_ = static_cast<double>(raw_width_) / raw_height_;
    }
}
