#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "Animator.h"
#include "CameraManager.h"
#include "HierarchyContainer.h"
#include "Object3D.h"
#include "ofxAssimpModelLoader.h"

namespace ift3100 {
class Application;
/**
 * Singleton class managing rendering of 3D objects.
 *
 * Features:
 * - Animations (w/ Animator).
 * - Multiple Camera Rendering (w/ Camera Manager).
 * - Hierarchy Manipulation (w/ HierarchyContainer).
 * - RealTime bounding box computing & display (w/ computeBoundaryBox()).
*/
class Renderer3D {
	ofBoxPrimitive _boudaryBox;
	bool		   _showBoundary;

	static Renderer3D* _renderer3D;

protected:
	Renderer3D();

	void drawScene();

public:
	Renderer3D(const Renderer3D& renderer3D) = delete;
	~Renderer3D();

	Renderer3D& operator=(const Renderer3D&) = delete;

	static Renderer3D* Get();

	ofFbo selectedCameraFBO;
	bool  isCameraSelected;

	CameraManager				 cameraManager;
	Animator					 animator;
	HierarchyContainer<Object3D> hierarchy;

	void setup();
	void update();

	void draw();

	void deleteSelected();
	void computeBoundaryBox();

	void setMouseInput(bool enable);
	void toggleAxesCameras(bool enable);
	void setMainCameraOrtho(bool enable);
	void importFromPath(const std::string& filepath);
};
} // namespace ift3100

#endif //!IFT3100_RENDERER_3D_H