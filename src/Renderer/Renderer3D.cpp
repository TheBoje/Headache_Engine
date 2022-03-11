#include "Renderer3D.h"
#include "Application.h"

namespace ift3100 {
    Renderer3D::Renderer3D(Application& _application) : application(_application) {};

    void Renderer3D::setup() {
        ofEnableSmoothing();
		ofEnableDepthTest();
        cameraManager.setup();
        IFT_LOG << "done";
    }

    void Renderer3D::update() {
        cameraManager.update();
    }

    void Renderer3D::draw() {
		ofNoFill();
		ofSetColor(255);

        // Draw axis camera if enabled
        if (cameraManager.axesCamerasEnabled()) {
            for (int i = 0; i < NB_AXES_CAM; i++) {
                cameraManager.beginCamera(i);
                debugNode.draw();
                cameraManager.endCamera(i);
            }
        }
        // Draw main camera
        cameraManager.beginCamera(3);
        debugNode.draw();
        cameraManager.endCamera(3);
    }

    void Renderer3D::setMouseInput(bool enable) {
        cameraManager.setMouseInput(enable);
    }

    void Renderer3D::toggleAxesCameras(bool enabled) {
        cameraManager.toggleAxesCameras(enabled);
    }

    void Renderer3D::setMainCameraOrtho(bool ortho) {
        cameraManager.setMainCameraOrtho(ortho);
    }
}