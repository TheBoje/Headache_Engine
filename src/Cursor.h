#ifndef IFT_3100_CURSEUR_H
#define IFT_3100_CURSEUR_H

#include "InterfaceUtils.h"

namespace ift3100 {
    /**
     * Draw a custom cursor dependant on the mouse action.
    */
    class Cursor {
    public:
        void draw(int mouseX, int mouseY, MouseAction mouseAction) ;
    };
}

#endif //! IFT_3100_CURSEUR_H
