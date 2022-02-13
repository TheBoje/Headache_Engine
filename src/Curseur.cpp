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
void Curseur::dessiner_curseur(float x, float y) const
{
    // paramètres de dessin
    float length = 10.0f;
    float offset = 5.0f;

    // dessiner le curseur en rouge si un des boutons du périphérique de pointage est appuyé
    //if (is_mouse_button_pressed)
    //    ofSetColor(255, 0, 0);
    //else
        ofSetColor(255,25,25);

    // dessiner la forme vectorielle
    ofDrawLine(x + offset, y, x + offset + length, y);
    ofDrawLine(x - offset, y, x - offset - length, y);
    ofDrawLine(x, y + offset, x, y + offset + length);
    ofDrawLine(x, y - offset, x, y - offset - length);

}

