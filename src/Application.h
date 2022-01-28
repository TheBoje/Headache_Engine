#ifndef APPLICATION_H
#define APPLICATION_H

#include "ofMain.h"
#include "Renderer.h"

namespace ift3100 {
	class Application : public ofBaseApp {
	public:

	Renderer renderer;

	void setup();
	void update();
	void keyReleased(int key);
	void dragEvent(ofDragInfo dragInfo);
	void draw();
	void exit();

	void addImage(ofImage * image);
	};
}

#endif // !APPLICATION_H