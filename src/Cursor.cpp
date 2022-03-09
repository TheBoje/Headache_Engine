
#include "ofMain.h"
#include "Cursor.h"
using namespace ift3100;

// fonction qui dessine un curseur
void Cursor::draw(int mouseX, int mouseY, MouseAction mouseAction) {
    // paramètres de dessin
    float length = 10.0f;
    float offset = 5.0f;

    ofSetColor(255, 0, 0);
    switch (mouseAction) {
        case DrawPrimitive:
            ofSetColor(ofColor::red);
            // dessiner la forme vectorielle
            ofDrawLine(mouseX + offset, mouseY, mouseX + offset + length, mouseY);
            ofDrawLine(mouseX - offset, mouseY, mouseX - offset - length, mouseY);
            ofDrawLine(mouseX, mouseY + offset, mouseX, mouseY + offset + length);
            ofDrawLine(mouseX, mouseY - offset, mouseX, mouseY - offset - length);
            break;
        // case DrawPrimitive:
        //     ofDrawLine(x + offset*2, y, x + offset + length, y);
        //     ofDrawLine(x - offset*2, y, x - offset - length, y);
        //     ofDrawLine(x, y + offset*2, x, y + offset + length);
        //     ofDrawLine(x, y - offset*2, x, y - offset - length);

            break;
        case None:
            ofSetColor(ofColor::red);
            ofDrawCircle(mouseX, mouseY, length);
            break;

        default:
            ofLog() << "<renderer::draw> unknown type of cursor";
            break;
    }
}

