
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
    int size = 30;

    ofHideCursor();
    switch (mouseAction) {
        case DrawPrimitive:
            ofSetColor(ofColor::white);
            cursorDraw.draw(mouseX - size, mouseY- size, size, size);
            break;

        case None:
            cursorDefault.draw(mouseX - (size / 2), mouseY, size, size);
            break;

        default:
            ofLog() << "<renderer::draw> unknown type of cursor";
            break;
    }
}
