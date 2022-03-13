#include "Renderer3D.h"
#include "Application.h"

namespace ift3100 {
Renderer3D::Renderer3D(Application& _application)
	: application(_application) { }

void Renderer3D::setup() {
	cameraManager.setup();

	// TODO: Temporaire
	hierarchy.setRoot(std::make_shared<Object3D>("root"));
	ofNode					  box;
	std::shared_ptr<Object3D> box_shared = std::make_shared<Object3D>("box", box);

	hierarchy.addChild(box_shared);
	// ----

	// Note: Uncomment me to enable animator testing, this is temporary until we implement a proper UI!
	// anim.setup();
	// anim.setTarget(box_shared->getNode());
	// anim.addKeyframe(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), 0);
	// anim.addKeyframe(ofVec3f(0, 100, 0), ofVec3f(90, 0, 0), 100);
	// anim.addKeyframe(ofVec3f(0, 100, -100), ofVec3f(0, 0, 0), 200);
	// anim.reset();
	// anim.resume();

	IFT_LOG << "done";
}

void Renderer3D::update() {
	cameraManager.update();
	anim.update();
}

/**
	 * Delete selected Object (in UI) from renderer hierarchy and UI.
	 *
	 * Note: Deleted primitive is added to undo stack.
	*/
void Renderer3D::deleteSelected() {
	// Delete each selected Object3D in hierarchy
	for (Hierarchy<Object3D>* selected : hierarchy.selected_nodes) {
		if (hierarchy.isRoot(*selected))
			hierarchy.clear();
		else
			delete selected;
	}

	hierarchy.selected_nodes.clear();
}

void Renderer3D::draw() {
	ofEnableDepthTest();

	// Draw axis camera if enabled
	if (cameraManager.axesCamerasEnabled()) {
		for (int i = 0; i < NB_AXES_CAM; i++) {
			cameraManager.beginCamera(i);
			hierarchy.mapChildren([](std::shared_ptr<Object3D> obj) {
				ofFill();
				// TODO(Louis): cleanup this mess
				if (obj->getType() == ObjectType::Mesh) {
					obj->getMesh()->drawFaces();
				} else {
					obj->getNode()->draw();
				}
			});
			cameraManager.endCamera(i);
		}
	}
	// Draw main camera
	cameraManager.beginCamera(3);
	hierarchy.mapChildren([](std::shared_ptr<Object3D> obj) {
		ofFill();
		// TODO(Louis): cleanup this mess
		if (obj->getType() == ObjectType::Mesh) {
			obj->getMesh()->drawFaces();
		} else {
			obj->getNode()->draw();
		}
	});
	cameraManager.endCamera(3);

	ofDisableDepthTest();
}

void Renderer3D::setMouseInput(bool enable) { cameraManager.setMouseInput(enable); }

void Renderer3D::toggleAxesCameras(bool enabled) { cameraManager.toggleAxesCameras(enabled); }

void Renderer3D::setMainCameraOrtho(bool ortho) { cameraManager.setMainCameraOrtho(ortho); }

} // namespace ift3100