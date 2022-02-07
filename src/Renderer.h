#ifndef RENDERER_H
#define RENDERER_H

#include "ofMain.h"
#include "Interface.h"

namespace ift3100 {
	class Renderer
	{
	public:

        ofTrueTypeFont font;
        ofColor background_color;
        ofColor stroke_color;
        ofRectangle bounding_box;
        string text;
        float stroke_weight;
        float line_offset;
        int font_size;

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