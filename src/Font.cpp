#include "Font.h"
#include "GameException.h"

#include <FTGL/ftgl.h>

Font::Font(const Font & f)
: font_file_(f.font_file_)
, font_(0)
, face_size_(40) {
    Initialize();
}
Font::Font(const std::string & font_file)
: font_file_(font_file)
, font_(0)
, face_size_(40) {
    Initialize();
}
Font::~Font() {
    delete font_;
}
void Font::Initialize() {
    font_ = new FTGLPixmapFont(font_file_.c_str());
    if (font_->Error()) {
        std::string msg = "Unable to load font from '" + font_file_ + "'";
        throw GameException(GameException::eFontLoadingError, msg);
    }
}
void Font::FaceSize(int size) {
    face_size_ = size;
    font_->FaceSize(face_size_);
}
void Font::Render(std::string text) {
    font_->Render(text.c_str());
}
void Font::Render(const char * text) {
    font_->Render(text);
}
