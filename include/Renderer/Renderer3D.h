#ifndef IFT3100_RENDERER_3D_H
#define IFT3100_RENDERER_3D_H

#include "AnimatorManager.h"
#include "CameraManager.h"
#include "Cubemap.h"
#include "HierarchyContainer.h"
#include "Object3D.h"
#include "ofxAssimpModelLoader.h"
#include "MaterialViewer.h"

namespace ift3100 {
class Application;

// enum IlluminationStyle { Default, Phong, Lambert, Gouraud, BlinnPhong };

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

	IlluminationStyle illumination;

	ofShader explodingShader;
	bool	 isExploding;

	ofFbo	  selectedCameraFBO;
	ofCamera* selectedCamera;

	CameraManager				 cameraManager;
	AnimatorManager				 animatorManager;
	HierarchyContainer<Object3D> hierarchy;
	Cubemap						 cubemap;

	std::vector<std::shared_ptr<Object3D>> lights;

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