#ifndef INTERFACE_UTILS_H
#define INTERFACE_UTILS_H

#include "Logger.h"

namespace ift3100 {
    // Available primitive to draw
    enum PrimitiveType { Point, Line, Rectangle, Ellipse, Triangle, Cross, Star };;

    // Supported mouse actions
    enum MouseAction { None, DrawPrimitive };

    /**
     * Miscs class for miscs methods!
    */
    class InterfaceUtils {
    public:
        static PrimitiveType intToPrimitiveType(int v) {
            switch (v) {
                case 0:
                    return Point;
                case 1:
                    return Line;
                case 2:
                    return Rectangle;
                case 3:
                    return Ellipse;
                case 4:
                    return Triangle;
                case 5:
                    return Cross;
                case 6:
                    return Star;
                default:
                    IFT_LOG << "unknown primitive type " << v << ", returning Line";
                    return Line;
            }
        }

        static const char * MouseActionToChar(MouseAction action) {
            switch (action) {
                case None:
                    return "None";
                case DrawPrimitive:
                    return "DrawPrimitive";
                /* NOTE: as some usage of this function requires certitude of
                * output, default is required. Please don't forget to implement this
                * when adding new MouseAction actions!
                */
                default:
                    ofLog(OF_LOG_WARNING) << "<InterfaceUtils::MouseActionToChar> unknown mouse action" << action << ", please update me.";
                    return "";
            }
        }
    };
}

#endif // !INTERFACE_UTILS_H
