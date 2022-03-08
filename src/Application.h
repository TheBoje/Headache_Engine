#ifndef APPLICATION_H
#define APPLICATION_H

#include "ofMain.h"
#include "Renderer.h"
#include "Interface.h"
#include "Curseur.h"

namespace ift3100 {
	class Application : public ofBaseApp {
	public:
	Application();
	Renderer renderer;
    Interface interface;

	bool isMouseDown;

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