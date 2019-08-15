#ifndef TEXT_RENDERER_FACTORY_HPP_
#define TEXT_RENDERER_FACTORY_HPP_

#include "TextRendererInterface.h"
#include "BitmapTextRenderer.hpp"
#include "FTGLTextRenderer.hpp"

class TextRendererFactory {
public:
    static TextRendererInterface * getTextRenderer() {

        return new FTGLTextRenderer();
    }
};

#endif // TEXT_RENDERER_FACTORY_HPP_
