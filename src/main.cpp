#include <iostream>
#include "ofMain.h"
#include "Application.h"
#include "ImageUtils.h"

int main(int argc, char**argv) {
	ofGLFWWindowSettings windowSettings;

	ofSetupOpenGL(1026, 1026, OF_WINDOW);
  	ofRunApp(new ift3100::Application());

	return EXIT_SUCCESS;
}
