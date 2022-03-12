#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "HierarchyContainer.h"
#include "CameraManager.h"
#include "ofLight.h"
#include "Object3D.h"

namespace ift3100 {

class Application;
class Renderer3D {
public:
	Application& application;
	Renderer3D(Application& _application);

	CameraManager cameraManager;

	ofLight light;

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