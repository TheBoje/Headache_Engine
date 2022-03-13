#include "Application.h"
#include "ImageUtils.h"
#include "Logger.h"
#include "ofMain.h"
#include <iostream>

int main(int argc, char** argv) {
	// ofSetLogLevel(OF_LOG_NOTICE);
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofGLFWWindowSettings windowSettings;
	windowSettings.setSize(1024, 1024);
	// windowSettings.setGLVersion(3, 2);

	ofCreateWindow(windowSettings);
	ofRunApp(new ift3100::Application());

	return EXIT_SUCCESS;
}
