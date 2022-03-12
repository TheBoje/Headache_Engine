#include "Application.h"
#include "ImageUtils.h"
#include "Logger.h"
#include "ofMain.h"
#include <iostream>

int main(int argc, char** argv) {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetupOpenGL(1026, 1026, OF_WINDOW);
	ofRunApp(new ift3100::Application());

	return EXIT_SUCCESS;
}
