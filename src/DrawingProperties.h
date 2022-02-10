#ifndef DRAWINGPROPERTIES_H
#define DRAWINGPROPERTIES_H

#include "ofColor.h"
#include "Constant.h"
#include "ofxGui.h"

namespace ift3100 {

    static ofColor BACKGROUND_COLOR(DEFAULT_BACKGROUND_COLOR);
    
    class DrawingProperties
    {
        
    public:

        ofColor STROKE_COLOR = DEFAULT_STROKE_COLOR;
        ofColor FILLED_COLOR = DEFAULT_FILLED_COLOR;
        
        float STROKE_WIDTH = DEFAULT_STROKE_WIDTH;

        void setStrokeColor(int r, int g, int b, int a) {
            STROKE_COLOR.r = r;
            STROKE_COLOR.g = g;
            STROKE_COLOR.b = b;
            STROKE_COLOR.a = a;
        }

        void setStrokeColor(ofColor color) {
            STROKE_COLOR = color;
        }

        void setFilledColor(int r, int g, int b, int a) {
            FILLED_COLOR.r = r;
            FILLED_COLOR.g = g;
            FILLED_COLOR.b = b;
            FILLED_COLOR.a = a;
        }

        void setFilledColor(ofColor color) {
            FILLED_COLOR = color;
        }
        
    };

}

#endif