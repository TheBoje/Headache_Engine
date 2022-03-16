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

	static Renderer3D* _renderer3D;

protected:
	Renderer3D();

public:
	Renderer3D(const Renderer3D& renderer3D) = delete;
	~Renderer3D();

	Renderer3D& operator=(const Renderer3D&) = delete;

	static Renderer3D* Get();

	CameraManager				 cameraManager;
	Animator					 anim;
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