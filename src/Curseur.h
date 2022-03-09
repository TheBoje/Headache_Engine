//
// Created by vincent on 2022-02-11.
//

#ifndef IFT_3100_CURSEUR_H
#define IFT_3100_CURSEUR_H

#pragma once

#include "ofMain.h"
#include "Interface.h"

namespace ift3100 {


    class Curseur {
    public:

        Interface interface;

        int choixCursor = interface.optionCursor;

        int mouse_press_x;
        int mouse_press_y;

        int souris_courant_x;
        int souris_courant_y;

        bool is_mouse_button_pressed;

        void dessiner_curseur(float x, float y, Interface::ModeCursor) const;

        void sourisDep(int x, int y);
    };
}


#endif //IFT_3100_CURSEUR_H
