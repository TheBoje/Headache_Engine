#include "Renderer3D.h"
#include "Application.h"

namespace ift3100 {
Renderer3D::Renderer3D(Application& _application)
	: application(_application) { }

void Renderer3D::setup() {
	ofEnableSmoothing();
	ofEnableDepthTest();
	cameraManager.setup();

	_showBoundary = false;

	// TODO: Temporaire
	hierarchy.setRoot(std::make_shared<Object3D>("root"));
	ofNode box;

	std::shared_ptr<Object3D> box_shared = std::make_shared<Object3D>("box", box);

	hierarchy.addChild(box_shared);
	light.setAmbientColor(ofColor(0, 60, 130));
	light.setPosition(ofVec3f(150, 150, 150));
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
	computeBoundaryBox();
}

void Renderer3D::computeBoundaryBox() {
	if (hierarchy.selected_nodes.size() == 0) {
		_showBoundary = false;
		return;
	}

	_showBoundary = true;
	ofVec3f maxPos;
	ofVec3f minPos;
	ofVec3f proportions;

	for (auto node : hierarchy.selected_nodes) {
		if (node->getRef()->getNode() == nullptr)
			continue;
		maxPos = node->getRef()->getNode()->getPosition();
		minPos = node->getRef()->getNode()->getPosition();
	}

	for (auto node : hierarchy.selected_nodes) {
		node->map([&](std::shared_ptr<Object3D> o) {
			if (o->getNode() == nullptr)
				return;

			ofVec3f nodepos = o->getNode()->getPosition();

			if (o->getType() == ObjectType::Primitive) {
				ofMesh		mesh		= ((of3dPrimitive*)o->getNode())->getMesh();
				std::size_t numVertices = mesh.getNumVertices();

				for (std::size_t i = 0; i < numVertices; i++) {
					ofVec3f vpos = mesh.getVertex(i) + o->getNode()->getPosition();

					maxPos.x = std::max(vpos.x, maxPos.x);
					maxPos.y = std::max(vpos.y, maxPos.y);
					maxPos.z = std::max(vpos.z, maxPos.z);

					minPos.y = std::min(vpos.y, minPos.y);
					minPos.z = std::min(vpos.z, minPos.z);
					minPos.x = std::min(vpos.x, minPos.x);
				}
			} else {
				maxPos.x = std::max(nodepos.x, maxPos.x);
				maxPos.y = std::max(nodepos.y, maxPos.y);
				maxPos.z = std::max(nodepos.z, maxPos.z);

				minPos.y = std::min(nodepos.y, minPos.y);
				minPos.z = std::min(nodepos.z, minPos.z);
				minPos.x = std::min(nodepos.x, minPos.x);
			}
		});
	}

	_boudaryBox.setPosition((maxPos + minPos) / 2);

	_boudaryBox.setWidth(std::abs(maxPos.x - minPos.x));
	_boudaryBox.setHeight(std::abs(maxPos.y - minPos.y));
	_boudaryBox.setDepth(std::abs(maxPos.z - minPos.z));

	IFT_LOG << "boudray box position " << _boudaryBox.getPosition();
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
	// Draw axis camera if enabled
	if (cameraManager.axesCamerasEnabled()) {
		for (int i = 0; i < NB_AXES_CAM; i++) {
			cameraManager.beginCamera(i);
			if (_showBoundary) {
				ofSetColor(150, 0, 0);
				_boudaryBox.drawWireframe();
				ofSetColor(255);
			}

			hierarchy.mapChildren([](std::shared_ptr<Object3D> obj) {
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
	if (_showBoundary) {
		ofSetColor(150, 0, 0);
		_boudaryBox.drawWireframe();
		ofSetColor(255);
	}
	hierarchy.mapChildren([](std::shared_ptr<Object3D> obj) {
		if (obj->getNode() != nullptr) {
			ofFill();
			obj->getNode()->draw();
		}
	});
	cameraManager.endCamera(3);
}

void Renderer3D::setMouseInput(bool enable) { cameraManager.setMouseInput(enable); }

void Renderer3D::toggleAxesCameras(bool enabled) { cameraManager.toggleAxesCameras(enabled); }

void Renderer3D::setMainCameraOrtho(bool ortho) { cameraManager.setMainCameraOrtho(ortho); }

} // namespace ift3100