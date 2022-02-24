#ifndef INTERFACE_UTILS_H
#define INTERFACE_UTILS_H

namespace ift3100 {
    // Available primitive to draw
    enum PrimitiveType { Line, Rectangle, Ellipse };;

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
                    return Line;
                case 1:
                    return Rectangle;
                case 2:
                    return Ellipse;
                default:
                    ofLog() << "<interfaceUtils::intToPrimitiveType> unknown primitive type " << v << ", returning Line";
                    return Line;
            }
        }
    };
}

#endif // !INTERFACE_UTILS_H
