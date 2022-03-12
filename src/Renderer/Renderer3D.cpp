#include "Renderer3D.h"
#include "Application.h"

namespace ift3100 {
    Renderer3D::Renderer3D(Application& _application) : application(_application)
    {}

    void Renderer3D::setup() {
        ofEnableSmoothing();
        ofEnableDepthTest();
        cameraManager.setup();

        // TODO: Temporaire
        hierarchy.setRoot(std::make_shared<Object3D>("root"));
        ofNode box;
        box.setPosition(0, 15, 0);
        hierarchy.addChild(std::make_shared<Object3D>("box", box));
        light.setAmbientColor(ofColor(0, 60, 130));
        light.setPosition(ofVec3f(150, 150, 150));
        // ----

        IFT_LOG << "done";
    }

    void Renderer3D::update() {
        cameraManager.update();
    }

	/**
	 * Delete selected Object (in UI) from renderer hierarchy and UI.
	 *
	 * Note: Deleted primitive is added to undo stack.
	*/
	void Renderer3D::deleteSelected() {
		// undoPrimitives.push(hierarchyPrimitives);

		// Delete each selected Object3D in hierarchy
		for(Hierarchy<Object3D> * selected : hierarchy.selected_nodes) {
			if(hierarchy.isRoot(*selected))
				hierarchy.clear();
			else
				delete selected;
		}

		hierarchy.selected_nodes.clear();
	}

    void Renderer3D::draw() {
        // Draw axis camera if enabled
        if (cameraManager.axesCamerasEnabled()) {
            for (int i = 0; i < NB_AXES_CAM; i++) {
                cameraManager.beginCamera(i);
                hierarchy.mapChildren([](std::shared_ptr<Object3D> obj){
                    if (obj->getNode() != nullptr) {
                        ofFill();
                        obj->getNode()->draw();
                    }
                });
                cameraManager.endCamera(i);
            }
        }
        // Draw main camera
        cameraManager.beginCamera(3);
        hierarchy.mapChildren([](std::shared_ptr<Object3D> obj){
            if (obj->getNode() != nullptr) {
                ofFill();
                obj->getNode()->draw();
            }
        });
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