#ifndef IFT_3100_INTERFACE_H
#define IFT_3100_INTERFACE_H

#include "ofMain.h"
#include "ofxImGui.h"
#include "DrawingProperties.h"
#include "InterfaceUtils.h"
#include <string.h>

namespace ift3100 {
class Interface {
    public:
        ofxImGui::Gui _gui;
        ofVec4f mousePos;

        bool primitiveUndo = false;
        bool primitiveRedo = false;

        float primitiveStrokeWidth;
        ImVec4 primitiveStrokeColor; // Dont question it.
        bool primitiveFill;
        ImVec4 primitiveFillColor; // Forget it.


        PrimitiveType drawMode;
        MouseAction mouseAction = None;
        
        void setup();
        void draw();
        void button_pressed();
    };
}

#endif //IFT_3100_INTERFACE_H