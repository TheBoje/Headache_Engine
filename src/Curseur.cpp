//
// Created by vincent on 2022-02-11.
//

#include "Curseur.h"
using namespace ift3100;

// Fonction appelée quand la position du curseur change
void Curseur::sourisDep(int x, int y)
{
    souris_courant_x = x;
    souris_courant_y = y;
}

// fonction qui dessine un curseur
void Curseur::dessiner_curseur(float x, float y, const Interface::ModeCursor mode) const {
    // paramètres de dessin
    float length = 10.0f;
    float offset = 5.0f;

    ofSetColor(255, 0, 0);
    switch (mode) {
        case interface.Mode_Select:
            // dessiner la forme vectorielle
            ofDrawLine(x + offset, y, x + offset + length, y);
            ofDrawLine(x - offset, y, x - offset - length, y);
            ofDrawLine(x, y + offset, x, y + offset + length);
            ofDrawLine(x, y - offset, x, y - offset - length);
            break;
        case interface.Mode_Move:
            ofDrawLine(x + offset*2, y, x + offset + length, y);
            ofDrawLine(x - offset*2, y, x - offset - length, y);
            ofDrawLine(x, y + offset*2, x, y + offset + length);
            ofDrawLine(x, y - offset*2, x, y - offset - length);

            break;
        case interface.Mode_Delete:

            break;

        default:
            ofLog() << "<renderer::draw> unknown type of cursor";
            break;
    }
}

