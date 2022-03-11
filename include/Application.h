#ifndef APPLICATION_H
#define APPLICATION_H

#include "ofMain.h"
#include "Interface.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "Cursor.h"
#include "ImageUtils.h"
#include "Hierarchy.h"
#include "Logger.h"
#include "CameraManager.h"

namespace ift3100 {
	class Application : public ofBaseApp {
	public:
    Interface interface;
	Renderer2D renderer2D;
	Renderer3D renderer3D;
	Cursor cursor;

	CameraManager cm;
	ofBoxPrimitive box;

	ofEasyCam cam1;

	bool isMouseDown;
	ofNode node;

	Application();
	~Application();

	void drawScene();

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
	};
}

#endif // !APPLICATION_H