#ifndef TEXT_H_
#define TEXT_H_

#include <unordered_map>

#include "font.h"

class Text {
public:
    Text()
    : font_(0)
    , default_font_(0)
    {
        default_font_ = new Font("fonts/mono/FreeMono.ttf");
        default_font_->FaceSize(40);
    }
    ~Text() {
        delete default_font_;
        for (auto f : fonts_) {
            delete f.second;
        }
    }
    void AddFont(std::string label, std::string path) {
        font_ = new Font(path);
        fonts_[label] = font_;
    }
    void UseFont(std::string label, int size) {
        auto found = fonts_.find(label);
        if (found != fonts_.end()) {
            font_ = fonts_[label];
        }
        else {
            font_ = default_font_;
        }
        font_->FaceSize(size);
    }
    void Render(int x, int y, std::string text) {
        glRasterPos2i(x, y);
        font_->Render(text);
    }

private:
    Font * font_;
    Font * default_font_;
    std::unordered_map<std::string, Font *> fonts_;
};

#endif  // TEXT_H_
