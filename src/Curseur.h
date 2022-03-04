//
// Created by vincent on 2022-02-11.
//

#ifndef IFT_3100_CURSEUR_H
#define IFT_3100_CURSEUR_H

#pragma once

#include "ofMain.h"

namespace ift3100 {
    class Curseur {

    public:

        int souris_courant_x;
        int souris_courant_y;

        void dessiner_curseur(float x, float y) const;

        void sourisDep(int x, int y);

    };
}


#endif //IFT_3100_CURSEUR_H
