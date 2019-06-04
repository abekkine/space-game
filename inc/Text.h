#ifndef TEXT_H_
#define TEXT_H_

#include <string>
#include <unordered_map>

#include "Font.h"

class Text {
public:
    Text();
    ~Text();
    void AddFont(std::string label, std::string path);
    void UseFont(std::string label, int size);
    void Render(int x, int y, std::string text);

private:
    Font * font_;
    Font * default_font_;
    std::unordered_map<std::string, Font *> fonts_;
};

#endif  // TEXT_H_
