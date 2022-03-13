#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "Animator.h"
#include "CameraManager.h"
#include "HierarchyContainer.h"
#include "Object3D.h"
#include "ofxAssimpModelLoader.h"

namespace ift3100 {
class Application;
class Renderer3D {
	ofBoxPrimitive _boudaryBox;
	bool		   _showBoundary;

public:
	Application& application;
	Renderer3D(Application& _application);

	CameraManager cameraManager;

	ofxAssimpModelLoader model;

	Animator anim;

	HierarchyContainer<Object3D> hierarchy;

	void setup();
	void update();

	void draw();

	void deleteSelected();
	void computeBoundaryBox();

	void setMouseInput(bool enable);
	void toggleAxesCameras(bool enabled);
	void setMainCameraOrtho(bool ortho);
};
} // namespace ift3100

#endif //!IFT3100_RENDERER_3D_H