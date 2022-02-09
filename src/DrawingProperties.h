#ifndef DRAWINGPROPERTIES_H
#define DRAWINGPROPERTIES_H

#include "ofColor.h"
#include "Constant.h"

namespace ift3100 {

    static ofColor BACKGROUND_COLOR(DEFAULT_BACKGROUND_COLOR);
    
    class DrawingProperties
    {
        
    public:

        ofColor STROKE_COLOR = DEFAULT_STROKE_COLOR;
        ofColor FILLED_COLOR = DEFAULT_FILLED_COLOR;
        
        float STROKE_WIDTH = DEFAULT_STROKE_WIDTH;
    };

}

#endif