#ifndef BITMAP_TEXT_RENDERER_HPP_
#define BITMAP_TEXT_RENDERER_HPP_

#include "TextRendererInterface.h"

#include <unordered_map>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <GL/glut.h>

class BitmapTextRenderer : public TextRendererInterface {
private:
    typedef std::unordered_map<int, void *> FontContainerType;
    enum {
        MAX_PRINT_SIZE = 1024,
    };

public:
    BitmapTextRenderer() {}
    ~BitmapTextRenderer() {}
    void Init() {
        default_font_= GLUT_BITMAP_8_BY_13;
        font_ = default_font_;
    }
    void Print(int x, int y, const char * fmt, ...) {
        glRasterPos2i(x, y);

        va_list args;
        va_start(args, fmt);
        Print(fmt, args);
        va_end(args);
    }
    void Print(const char * format, ...) {
        memset(print_buffer_, 0, MAX_PRINT_SIZE);

        va_list args;
        va_start(args, format);
        vsnprintf(print_buffer_, MAX_PRINT_SIZE, format, args);
        va_end(args);

        for (unsigned int i=0; i<strlen(print_buffer_); ++i) {
            glutBitmapCharacter(font_, print_buffer_[i]);
        }
    }
    void Print(std::string message) {
        for (char c : message) {
            glutBitmapCharacter(font_, c);
        }
    }

private:
    char print_buffer_[MAX_PRINT_SIZE];
    void * font_;
    void * default_font_;
    FontContainerType fonts_;
};

#endif // BITMAP_TEXT_RENDERER_HPP_
