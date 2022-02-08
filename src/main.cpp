#include <iostream>
#include "ofMain.h"
#include "Application.h"
#include "ImageUtils.h"

int main(int argc, char**argv) {
	ofGLFWWindowSettings windowSettings;

	ofSetupOpenGL(512, 512, OF_WINDOW);
  	ofRunApp(new ift3100::Application());

	return EXIT_SUCCESS;
}
