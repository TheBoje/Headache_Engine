#ifndef APPLICATION_H
#define APPLICATION_H

#include "CameraManager.h"
#include "Cursor.h"
#include "Hierarchy.h"
#include "ImageUtils.h"
#include "Interface.h"
#include "Logger.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "ofMain.h"

namespace ift3100 {
class Application : public ofBaseApp {
public:
	Interface  interface;
	Renderer2D renderer2D;
	Renderer3D renderer3D;
	Cursor	   cursor;

	bool isMouseDown;

	Application();
	~Application();

	void setup();
	void update();
	void keyReleased(int key);
	void keyPressed(int key);
	void dragEvent(ofDragInfo dragInfo);
	void draw();
	void exit();

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);

	void drawPrimitivePreview();
	void rendererUndo();
	void rendererRedo();
	void exportRender(std::string filename);
	void import3DObj(std::string filename);
};
} // namespace ift3100

#endif // !APPLICATION_H