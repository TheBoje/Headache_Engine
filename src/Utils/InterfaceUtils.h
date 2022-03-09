#ifndef INTERFACE_UTILS_H
#define INTERFACE_UTILS_H

namespace ift3100 {
    // Available primitive to draw
    enum PrimitiveType { Line, Rectangle, Ellipse };

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
