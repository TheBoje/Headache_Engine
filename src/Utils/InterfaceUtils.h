#ifndef INTERFACE_UTILS_H
#define INTERFACE_UTILS_H

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
                    ofLog() << "<interfaceUtils::intToPrimitiveType> unknown primitive type " << v << ", returning Line";
                    return Line;
            }
        }
    };
}

#endif // !INTERFACE_UTILS_H
