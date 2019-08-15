#ifndef TEXT_RENDERER_INTERFACE_H_
#define TEXT_RENDERER_INTERFACE_H_

#include <string>

class TextRendererInterface {
public:
    virtual ~TextRendererInterface() {}
    virtual void Init() = 0;
    virtual void Print(const char * fmt, ...) = 0;
    virtual void Print(int x, int y, const char * fmt, ...) = 0;
    virtual void Print(std::string message) = 0;
    virtual void UseFont(int index, int size) = 0;
    virtual void AddFont(int index, std::string path) = 0;
    virtual void AddFont(int index, int font_id) = 0;
};

#endif // TEXT_RENDERER_INTERFACE_H_
