#ifndef IFT_3100_INTERFACE_H
#define IFT_3100_INTERFACE_H

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include "ofxGuiExtended"
#include "Renderer.h"
#include <string.h>

namespace ift3100 {
    class Interface {

    public:

        ofxPanel gui;

        ofxButton buttonSelection;
        ofxButton buttonDeplacement;
        ofxButton buttonSuppression;
        ofxButton buttonHistogramme;
        ofxButton buttonImportimage;


        ofxGuiGroup OutilsDessin;
        ofxGuiGroup odPrimitives;

        ofxGuiGroup PointVue;
        ofxButton buttonPV1;
        ofxButton buttonPV2;
        ofxButton buttonPV3;

        ofxGuiGroup Projection;
        ofxButton buttonPerspective;
        ofxButton buttonOrthogonale;

        ofxIntSlider xInterface;
        ofxIntSlider yInterface;
        ofxIntSlider rayonInterface;
        ofxIntSlider rInterface;
        ofxIntSlider gInterface;
        ofxIntSlider bInterface;
        ofxIntSlider aInterface;

        ofParameter<bool> checkboxBoiteDelimitation;


        int width = 512;
        int height = 512;


        ofParameter<ofColor> color_picker_background;
        ofParameter<ofColor> color_picker_stroke;
        ofParameter<ofColor> color_picker_object;

        ofParameter<float> slider_stroke_weight;

        ofParameter<string> textbox;

        ofParameter<bool> checkbox;


        void construire_interface();

        void button_pressed();

    };
}





#endif //IFT_3100_INTERFACE_H