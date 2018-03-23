#ifndef FONT_H_
#define FONT_H_

#include <FTGL/ftgl.h>

#include <string>

#include "game_exception.h"

class Font {
public:
    Font(const Font & f)
    : font_file_(f.font_file_)
    , font_(0)
    , face_size_(40) {
        Initialize();
    }
    explicit Font(const std::string & font_file)
    : font_file_(font_file)
    , font_(0)
    , face_size_(40) {
        Initialize();
    }
    ~Font() {
        delete font_;
    }
    void Initialize() {
        font_ = new FTGLPixmapFont(font_file_.c_str());
        if (font_->Error()) {
            std::string msg = "Unable to load font from '" + font_file_ + "'";
            throw GameException(GameException::eFontLoadingError, msg);
        }
    }
    void FaceSize(int size) {
        face_size_ = size;
        font_->FaceSize(face_size_);
    }
    void Render(std::string text) {
        font_->Render(text.c_str());
    }
    void Render(const char * text) {
        font_->Render(text);
    }

private:
     std::string font_file_;
     FTGLPixmapFont * font_;
     int face_size_;
};

#endif  // FONT_H_
