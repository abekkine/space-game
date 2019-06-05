#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL.h>
#include <SDL_image.h>

#include <string>

class Texture {
public:
    explicit Texture(const std::string & file_name);
    ~Texture();
    void Render();

private:
    SDL_Surface * LoadFile();
    void GenerateTexture();
    void Normalize();

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
