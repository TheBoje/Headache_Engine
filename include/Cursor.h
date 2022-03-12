#ifndef IFT_3100_CURSEUR_H
#define IFT_3100_CURSEUR_H

#include "Constant.h"
#include "InterfaceUtils.h"

namespace ift3100 {
/**
     * Draw a custom cursor dependant on the mouse action.
    */
class Cursor {
public:
	void setup();
	void draw(int mouseX, int mouseY, MouseAction mouseAction, PrimitiveType type);

	ofImage cursorDraw;
	ofImage cursorDefault;
	ofImage cursorCross;
	ofImage cursorStar;
};
} // namespace ift3100

#endif //! IFT_3100_CURSEUR_H
