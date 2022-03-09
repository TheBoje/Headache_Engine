#ifndef IFT_3100_INTERFACE_H
#define IFT_3100_INTERFACE_H

#include "ofMain.h"
#include "ofxImGui.h"
#include "DrawingProperties.h"
#include "InterfaceUtils.h"
#include "Hierarchy.h"
#include "Renderer.h"
#include "ofImage.h"
#include "ImageUtils.h"
#include "Logger.h"

#include <string.h>

namespace ift3100 {
class Application;
class Interface {
    public:
        Application& application; // Reference to main application for callbacks

        ofxImGui::Gui _gui;
        ofVec4f mousePos;

        unsigned int ** _rgb;

        float primitiveStrokeWidth;
        ImVec4 primitiveStrokeColor; // Dont question it.
        bool primitiveFill;
        ImVec4 primitiveFillColor; // Forget it.


        PrimitiveType drawMode;
        MouseAction mouseAction = None;

        ofTexture textureSource;
        GLuint textureSourceID;
        ofImage image;
        bool isHistComputed;
        char imageRenderName[64];

        Interface(Application& _application);

        void setup();
        void loadImage(std::string path);
        void imageUI();
        void drawingUI();
        void draw();
        void button_pressed();
    };
}

#endif //IFT_3100_INTERFACE_H