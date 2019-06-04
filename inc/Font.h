#ifndef FONT_H_
#define FONT_H_

#include <FTGL/ftgl.h>

#include <string>

class Font {
public:
    Font(const Font & f);
    explicit Font(const std::string & font_file);
    ~Font();
    void Initialize();
    void FaceSize(int size);
    void Render(std::string text);
    void Render(const char * text);

private:
     std::string font_file_;
     FTGLPixmapFont * font_;
     int face_size_;
};

#endif  // FONT_H_
