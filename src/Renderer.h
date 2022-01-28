#ifndef RENDERER_H
#define RENDERER_H

#include "ofMain.h"

namespace ift3100 {
	class Renderer
	{
	public:

		float _time_current;
		float _time_last;
		float _time_elapsed;

		ofImage * _image;

		void setup();
		void update();
		void draw();
	};
}

#endif // !RENDERER_H