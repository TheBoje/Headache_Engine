#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "HierarchyContainer.h"
#include "CameraManager.h"

namespace ift3100 {
    class Application;
    class Renderer3D {
        public:
            Application& application;

            HierarchyContainer<ofNode> hierarchy;
            ofNode debugNode;
            CameraManager cameraManager;

            Renderer3D(Application& _application);
            void setup();
            void update();
            void draw();

            void setMouseInput(bool enable);
            void toggleAxesCameras(bool enabled);
            void setMainCameraOrtho(bool ortho);
    };
}

#endif //!IFT3100_RENDERER_3D_H