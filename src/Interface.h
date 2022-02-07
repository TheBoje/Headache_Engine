#ifndef IFT_3100_INTERFACE_H
#define IFT_3100_INTERFACE_H

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Renderer.h"

namespace ift3100 {
    class Interface {

    public:


        ofxPanel gui;
        ofxIntSlider xInterface;
        ofxIntSlider yInterface;
        ofxIntSlider rayonInterface;
        ofxIntSlider rInterface;
        ofxIntSlider gInterface;
        ofxIntSlider bInterface;
        ofxIntSlider aInterface;

        ofxGuiGroup texture1;
        ofxGuiGroup texture2;

        ofParameter<ofColor> color_picker_background;
        ofParameter<ofColor> color_picker_stroke;

        ofParameter<float> slider_stroke_weight;

        ofParameter<string> textbox;

        ofParameter<bool> checkbox;

        ofxButton button;
    };
}





#endif //IFT_3100_INTERFACE_H