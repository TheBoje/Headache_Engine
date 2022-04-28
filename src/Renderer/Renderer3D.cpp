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
	Model::setup();
	cameraManager.setup();
	animatorManager.setup();
	cubemap.setup("../../data/Skybox/sprite.png", 1024);

	_showBoundary = false;

	// TODO: Temporaire
	hierarchy.setRoot(std::make_shared<Object3D>("root"));
	ofNode box;
	std::shared_ptr<Object3D> box_shared = std::make_shared<Object3D>("box", box);
	hierarchy.addChild(box_shared);
	// SHADERS
	explodingShader.load("../../src/Shaders/Exploding/exploding.vert.glsl",
		"../../src/Shaders/Exploding/exploding.frag.glsl",
		"../../src/Shaders/Exploding/exploding.geom.glsl");

	isExploding = false;

	// ----
	// Note: Uncomment me to enable animator testing, this is temporary until we implement a proper UI!
	// animator.setup();
	// animator.setTarget(box_shared->getNode());
	// animator.addKeyframe(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), 0);
	// animator.addKeyframe(ofVec3f(0, 100, 0), ofVec3f(90, 0, 0), 100);
	// animator.addKeyframe(ofVec3f(0, 100, -100), ofVec3f(0, 0, 0), 200);
	// animator.reset();
	// animator.resume();

	animatorManager.addAnimator(box_shared);

	selectedCamera = nullptr;
	ofBoxPrimitive bounds = ofBoxPrimitive(300, 300, 300);

	IFT_LOG << "done";
}

void Renderer3D::update() {
	cameraManager.update();
	animatorManager.update();
	hierarchy.update();
	computeBoundaryBox();

	selectedCamera = nullptr;
	// Search in selected nodes if there is a camera
	for (auto selected : hierarchy.selected_nodes) {
		if (selected->getRef()->getType() == ObjectType::Camera) {
			selectedCamera = ((ofCamera*)selected->getRef()->getNode());
		}
	}

	// Allocate the FBO if there is a camera with the FBO not allocated, clear it instead
	// The allocation is set to CAMERA_WIDTH and the height is find with the aspect ratio.
	if (selectedCamera != nullptr && !selectedCameraFBO.isAllocated()) {
		selectedCameraFBO.allocate(CAMERA_WIDTH, (CAMERA_WIDTH / selectedCamera->getAspectRatio()));
	} else if (selectedCamera == nullptr && selectedCameraFBO.isAllocated()) {
		selectedCameraFBO.clear();
	}
}

/**
 * Compute bounding box containing every 3D objects in the scene.
 *
*/
void Renderer3D::computeBoundaryBox() {
	// Check if there is at least one node selected
	if (hierarchy.selected_nodes.size() == 0) {
		_showBoundary = false;
		return;
	}

	_showBoundary = true;
	ofVec3f maxPos;
	ofVec3f minPos;
	ofVec3f scale;

	// Init the maximum and minimum position to the first node position
	maxPos = hierarchy.selected_nodes.at(0)->getRef()->getNode()->getPosition();
	minPos = hierarchy.selected_nodes.at(0)->getRef()->getNode()->getPosition();

	// For each selected nodes and there children, go through all meshes vertices
	// in order to get the maximum and minimum position.
	for (auto node : hierarchy.selected_nodes) {
		node->map([&](std::shared_ptr<Object3D> object) {
			if (object->getNode() == nullptr)
				return;

			// Get the center and the rotation (needed for rotated meshes to compute global vertex position)
			ofVec3f nodePos = object->getNode()->getPosition();
			ofVec3f nodeScale = object->getNode()->getScale();
			ofVec3f nodRotation = object->getNode()->getOrientationEulerDeg();

			if (object->getType() == ObjectType::Model3D) {
				ofMesh mesh = ((of3dPrimitive*)object->getNode())->getMesh();
				std::size_t numVertices = mesh.getNumVertices();

				// Go through all vertices of the mesh if it exist
				for (std::size_t i = 0; i < numVertices; i++) {
					ofVec3f vpos = (mesh.getVertex(i) * nodeScale) + nodePos;
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
				// If the Object3D is not a Primitive type, then just get the node position
				// because it don't get an ofMesh attribute
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
		if (hierarchy.isRoot(*selected)) {
			IFT_LOG_WARNING << "clearing hierarchy";
			hierarchy.clear();
		} else {
			IFT_LOG_WARNING << "deleting selected";
			delete selected;
		}
	}

	hierarchy.selected_nodes.clear();
}

void Renderer3D::drawScene() {
	ofFill();
	v3d.draw();
	hierarchy.mapChildren([&](std::shared_ptr<Object3D> obj) {
		// Check if the obj is selected and apply the exploding shader if so
		bool isSelected = false;
		for (Hierarchy<Object3D>* selected : hierarchy.selected_nodes) {
			if (selected->getRef() == obj) {
				isSelected = true;
				break;
			}
		}

		if (isExploding) {
			explodingShader.begin();
			obj->draw(isSelected);
			explodingShader.end();
		} else {
			obj->draw(isSelected);
		}
	});

	if (selectedCamera != nullptr)
		selectedCamera->drawFrustum();
}

void Renderer3D::draw() {
	ofEnableDepthTest();

	// Store result of selected camera in the FBO
	if (selectedCamera != nullptr) {
		selectedCameraFBO.begin();
		// Avoid residues in the FBO
		ofClear(120, 120, 120, 255);

		selectedCamera->begin();
		cubemap.draw(selectedCamera->getGlobalPosition());
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
			cubemap.draw(cameraManager.get(i).getGlobalPosition());

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
	cubemap.draw(cameraManager.get(3).getGlobalPosition());
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
	model.load(filepath);
	// FIXME: Merge the meshes in 1, and keep the ofNode as the offset point.
	// Update in Object3D is required for this fix.

	if (model.getMeshCount() == 1) {
		IFT_LOG << "loading " << model.getMeshCount() << " meshes";
		hierarchy.addChild(std::make_shared<Object3D>(model.getMeshNames().at(0), model.getMesh(0), model.getTextureForMesh(0)));
	} else if (model.getMeshCount() > 1) {
		IFT_LOG << "loading " << model.getMeshCount() << " meshes";

		std::shared_ptr<Object3D> parent = std::make_shared<Object3D>(filepath, ofNode());
		std::vector<std::shared_ptr<Object3D>> children;
		children.reserve(model.getMeshCount());

		for (size_t i = 0; i < model.getMeshCount(); i++) {
			children.emplace_back(std::make_shared<Object3D>(model.getMeshNames().at(i), model.getMesh(i), model.getTextureForMesh(i)));
		}
		hierarchy.addChildren(children, parent);
	} else {
		IFT_LOG_ERROR << "import failed, object doesn't have a mesh";
	}
}

} // namespace ift3100