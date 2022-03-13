#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "CameraManager.h"
#include "HierarchyContainer.h"
#include "Object3D.h"
#include "ofxAssimpModelLoader.h"

namespace ift3100 {
class Application;
class Renderer3D {
public:
	Application& application;
	Renderer3D(Application& _application);

	CameraManager cameraManager;

	ofxAssimpModelLoader model;

	HierarchyContainer<Object3D> hierarchy;

	void setup();
	void update();

	void draw();

	void deleteSelected();

	void setMouseInput(bool enable);
	void toggleAxesCameras(bool enabled);
	void setMainCameraOrtho(bool ortho);
};
} // namespace ift3100

#endif //!IFT3100_RENDERER_3D_H