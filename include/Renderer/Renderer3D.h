#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "HierarchyContainer.h"

namespace ift3100 {
    class Application;
    class Renderer3D {
        public:
            Application& application;

            HierarchyContainer<ofNode> hierarchy; 

            Renderer3D(Application& _application);
            void setup();
            void update();
            void draw();
    };
}

#endif //!IFT3100_RENDERER_3D_H