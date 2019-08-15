#ifndef TEXT_MANAGER_HPP_
#define TEXT_MANAGER_HPP_

#include "TextRendererInterface.h"

#include <unordered_map>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <FTGL/ftgl.h>

class FTGLTextRenderer : public TextRendererInterface {
private:
    typedef std::unordered_map<int, FTGLPixmapFont *> FontContainerType;
    enum {
        MAX_PRINT_SIZE = 1024,
    };

public:
    FTGLTextRenderer() {}
    ~FTGLTextRenderer() {
        for (auto f : fonts_) {
            delete f.second;
        }
        delete default_font_;
    }
    void Init() {
        default_font_ = new FTGLPixmapFont("ubuntu_mono.ttf");

        if (default_font_->Error()) {
            throw std::string("Unable to load default font!");
        }

        default_font_->FaceSize(24);

        font_ = default_font_;
    }
    void Print(int x, int y, const char * fmt, ...) {
        glRasterPos2i(x, y);
        va_list args;
        va_start(args, fmt);
        // BUG : Does not working...
        Print(fmt, args);
        va_end(args);
    }
    void Print(const char * fmt, ...) {
        memset(print_buffer_, 0, MAX_PRINT_SIZE);

        va_list args;
        va_start(args, fmt);
        vsnprintf(print_buffer_, MAX_PRINT_SIZE, fmt, args);
        va_end(args);

        font_->Render(print_buffer_);
    }
    void Print(std::string message) {
        font_->Render(message.c_str());
    }
    void UseFont(int index, int size) {
        FontContainerType::const_iterator it;
        it = fonts_.find(index);
        if (it != fonts_.end()) {
            font_ = fonts_[index];
        }
        else {
            font_ = default_font_;
        }
        font_->FaceSize(size);
    }
    void AddFont(int index, std::string path) {
        font_ = new FTGLPixmapFont(path.c_str());
        if (font_->Error()) {
            throw std::string("Unable to find given font!");
        }

        fonts_[index] = font_;
    }
    void AddFont(int index, int font_id) {
        // Will not be implemented.
        (void)index;
        (void)font_id;
    }

private:
    char print_buffer_[MAX_PRINT_SIZE];
    FTGLPixmapFont * font_;
    FTGLPixmapFont * default_font_;
    FontContainerType fonts_;
};

#endif // TEXT_MANAGER_HPP_
