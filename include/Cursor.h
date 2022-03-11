#ifndef IFT_3100_CURSEUR_H
#define IFT_3100_CURSEUR_H

#include "InterfaceUtils.h"
#include "Constant.h"

namespace ift3100 {
    /**
     * Draw a custom cursor dependant on the mouse action.
    */
    class Cursor {
    public:
        void setup();
        void draw(int mouseX, int mouseY, MouseAction mouseAction, PrimitiveType type) ;

        ofImage cursorDraw;
        ofImage cursorDefault;
        ofImage cursorCross;
        ofImage cursorStar;
    };
}

#endif //! IFT_3100_CURSEUR_H
