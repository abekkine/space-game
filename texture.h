#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GLFW/glfw3.h>
#include <SDL.h>
#include <SDL_image.h>

#include <string>

#include "game_exception.h"

// TODO Implement Texture class +game +display +render id:13 gh:15 ic:gh
// - Handles textures.
class Texture {
 public:
    explicit Texture(std::string file_name)
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
    ~Texture() {
        glDeleteTextures(1, &texture_id_);
    }
    void Render() {
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

 private:
    SDL_Surface * LoadFile() {
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
    void GenerateTexture() {
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
    void Normalize() {
        if (raw_width_ > raw_height_) {
            width_ = 1.0;
            height_ = static_cast<double>(raw_height_) / raw_width_;
        } else {
            height_ = 1.0;
            width_ = static_cast<double>(raw_width_) / raw_height_;
        }
    }

 private:
    double width_;
    double height_;
    int raw_width_;
    int raw_height_;
    std::string file_name_;
    uint8_t bytes_per_pixel_;
    unsigned int texture_format_;
    unsigned int texture_id_;
    void * pixels_;
};

#endif  // TEXTURE_H_
