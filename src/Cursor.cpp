
#include "ofMain.h"
#include "Cursor.h"
using namespace ift3100;

void Cursor::setup(){
    cursorDefault.load("../../data/default.png");
    cursorDraw.load("../../data/draw.png");
}

// fonction qui dessine un curseur
void Cursor::draw(int mouseX, int mouseY, MouseAction mouseAction) {
    // paramètres de dessin
    float offset = 50.0f;
    int size = 30;


    switch (mouseAction) {
        case DrawPrimitive:
            ofHideCursor();

            cursorDraw.draw(mouseX-offset, mouseY-offset, size, size);

            break;

        case None:
            ofHideCursor();
            ofSetColor(ofColor::white);
            cursorDefault.draw(mouseX-10, mouseY, size, size);
            break;

        default:
            ofLog() << "<renderer::draw> unknown type of cursor";
            break;
    }
}

