#include "Renderer3D.h"
#include "Application.h"

namespace ift3100 {

Renderer3D* Renderer3D::_renderer3D = nullptr;

Renderer3D::Renderer3D() { }

Renderer3D::~Renderer3D() { }

Renderer3D* Renderer3D::Get() {
	if (_renderer3D == nullptr) {
		_renderer3D = new Renderer3D();
	}
	return _renderer3D;
}

void Renderer3D::setup() {
	cameraManager.setup();

	selectedCameraFBO.allocate(1024, 720);
	_showBoundary = false;

	// TODO: Temporaire
	hierarchy.setRoot(std::make_shared<Object3D>("root"));
	ofNode					  box;
	std::shared_ptr<Object3D> box_shared = std::make_shared<Object3D>("box", box);
	
	hierarchy.addChild(box_shared);
	// ----

	// Note: Uncomment me to enable animator testing, this is temporary until we implement a proper UI!
	// animator.setup();
	// animator.setTarget(box_shared->getNode());
	// animator.addKeyframe(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), 0);
	// animator.addKeyframe(ofVec3f(0, 100, 0), ofVec3f(90, 0, 0), 100);
	// animator.addKeyframe(ofVec3f(0, 100, -100), ofVec3f(0, 0, 0), 200);
	// animator.reset();
	// animator.resume();

	IFT_LOG << "done";
}

void Renderer3D::update() {
	cameraManager.update();
	computeBoundaryBox();
	animator.update();
}

/**
 * Compute bounding box containing every 3D objects in the scene.
 *
*/
void Renderer3D::computeBoundaryBox() {
	if (hierarchy.selected_nodes.size() == 0) {
		_showBoundary = false;
		return;
	}

	_showBoundary = true;
	ofVec3f maxPos;
	ofVec3f minPos;
	ofVec3f scale;

	for (auto node : hierarchy.selected_nodes) {
		if (node->getRef()->getNode() == nullptr)
			continue;
		maxPos = node->getRef()->getNode()->getPosition();
		minPos = node->getRef()->getNode()->getPosition();
	}

	for (auto node : hierarchy.selected_nodes) {
		node->map([&](std::shared_ptr<Object3D> object) {
			if (object->getNode() == nullptr)
				return;

			ofVec3f nodePos		= object->getNode()->getPosition();
			ofVec3f nodRotation = object->getNode()->getOrientationEulerDeg();

			if (object->getType() == ObjectType::Primitive) {
				ofMesh		mesh		= ((of3dPrimitive*)object->getNode())->getMesh();
				std::size_t numVertices = mesh.getNumVertices();

				for (std::size_t i = 0; i < numVertices; i++) {
					ofVec3f vpos = mesh.getVertex(i) + nodePos;
					vpos.rotate(nodRotation.x, nodePos, ofVec3f(1, 0, 0));
					vpos.rotate(nodRotation.y, nodePos, ofVec3f(0, 1, 0));
					vpos.rotate(nodRotation.z, nodePos, ofVec3f(0, 0, 1));

					maxPos.x = std::max(vpos.x, maxPos.x);
					maxPos.y = std::max(vpos.y, maxPos.y);
					maxPos.z = std::max(vpos.z, maxPos.z);

					minPos.y = std::min(vpos.y, minPos.y);
					minPos.z = std::min(vpos.z, minPos.z);
					minPos.x = std::min(vpos.x, minPos.x);
				}
			} else {
				maxPos.x = std::max(nodePos.x, maxPos.x);
				maxPos.y = std::max(nodePos.y, maxPos.y);
				maxPos.z = std::max(nodePos.z, maxPos.z);

				minPos.y = std::min(nodePos.y, minPos.y);
				minPos.z = std::min(nodePos.z, minPos.z);
				minPos.x = std::min(nodePos.x, minPos.x);
			}
		});
	}

	_boudaryBox.setPosition((maxPos + minPos) / 2);

	_boudaryBox.setWidth(std::abs(maxPos.x - minPos.x));
	_boudaryBox.setHeight(std::abs(maxPos.y - minPos.y));
	_boudaryBox.setDepth(std::abs(maxPos.z - minPos.z));
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

void Renderer3D::drawScene() {
	hierarchy.mapChildren([](std::shared_ptr<Object3D> obj) {
		ofFill();
		obj->getNode()->draw();
	});
}

void Renderer3D::draw() {
	ofEnableDepthTest();

	ofCamera * selectedCamera = nullptr;
	isCameraSelected = false;
	for(auto selected : hierarchy.selected_nodes) {
		if(selected->getRef()->getType() == ObjectType::Camera) {
			selectedCamera = ((ofCamera *)selected->getRef()->getNode());
			isCameraSelected = true;
			break;
		}
	}

	selectedCameraFBO.begin();
	ofClear(255, 255, 255, 0);
	selectedCameraFBO.end();

	if(isCameraSelected){
		selectedCameraFBO.begin();
		selectedCamera->begin();
		drawScene();
		selectedCamera->end();
		selectedCameraFBO.end();
	}

	// Draw axis camera if enabled
	if (cameraManager.axesCamerasEnabled()) {
		for (int i = 0; i < NB_AXES_CAM; i++) {
			cameraManager.beginCamera(i);
			if (_showBoundary) {
				ofSetColor(150, 0, 0);
				_boudaryBox.drawWireframe();
				ofSetColor(255);
			}

			if(isCameraSelected)
				selectedCamera->drawFrustum();

			
			drawScene();
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

	if(isCameraSelected)
		selectedCamera->drawFrustum();

	drawScene();

	cameraManager.endCamera(3);

	ofDisableDepthTest();
}

/**
 * Update the mouse input status for every cameras.
 * Note: cameras mouse input is disabled when UI / 2D drawing is in use, and enabled back when it's done.
 *
 * @param enable new state of the mouse input (true:enable mouse input).
*/
void Renderer3D::setMouseInput(bool enable) {
	cameraManager.setMouseInput(enable);
}

/**
 * @param enable new state of the multiple-axis cameras (true:enable multiple-axis cameras).
*/
void Renderer3D::toggleAxesCameras(bool enable) {
	cameraManager.toggleAxesCameras(enable);
}

/**
 * @param enable new state of the camera (true: ortho, false: perspective).
*/
void Renderer3D::setMainCameraOrtho(bool enable) {
	cameraManager.setMainCameraOrtho(enable);
}

/**
 * Load 3D asset into the renderer from a file.
 * Note: filepath is from `bin/data/`.
 *
 * @param filepath path of the 3D asset (eg. "jar_jar.obj", for "bin/data/jar_jar.obj").
*/
void Renderer3D::importFromPath(const std::string& filepath) {
	IFT_LOG << "Trying to import bin/data/" << filepath;
	ofxAssimpModelLoader model;
	model.loadModel(filepath);
	// FIXME: Merge the meshes in 1, and keep the ofNode as the offset point.
	// Update in Object3D is required for this fix.
	if (model.getMeshCount() >= 1) {
		IFT_LOG << "loading " << model.getMeshCount() << " meshes";
		for (int i = 0; i < model.getMeshCount(); i++) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>(filepath + std::to_string(i), model.getMesh(i)));
		}
	} else {
		IFT_LOG_ERROR << "import failed, object doesn't have a mesh";
	}
}

} // namespace ift3100