#include "Text.h"
#include "Font.h"

#include <GLFW/glfw3.h>

Text::Text()
: font_(0)
, default_font_(0)
{
    default_font_ = new Font("../fonts/mono/FreeMono.ttf");
    default_font_->FaceSize(40);
}
Text::~Text() {
    delete default_font_;
    for (auto f : fonts_) {
        delete f.second;
    }
}
void Text::AddFont(std::string label, std::string path) {
    font_ = new Font(path);
    fonts_[label] = font_;
}
void Text::UseFont(std::string label, int size) {
    auto found = fonts_.find(label);
    if (found != fonts_.end()) {
        font_ = fonts_[label];
    }
    else {
        font_ = default_font_;
    }
    font_->FaceSize(size);
}
void Text::Render(int x, int y, std::string text) {
    glRasterPos2i(x, y);
    font_->Render(text);
}
