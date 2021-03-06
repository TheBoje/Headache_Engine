#ifndef DRAWINGPROPERTIES_H
#define DRAWINGPROPERTIES_H

#include "Constant.h"
#include "ofColor.h"

namespace ift3100 {

static ofColor BACKGROUND_COLOR(DEFAULT_BACKGROUND_COLOR);

/**
     * @brief Properties for vectors drawing.
     * Contains the Stroke color, stroke width and the filled color.
     * Each vector primitive is intented to get one DrawingProperties to describe it. 
     */
class DrawingProperties {
public:
	ofColor STROKE_COLOR = DEFAULT_STROKE_COLOR;
	ofColor FILLED_COLOR = DEFAULT_FILL_COLOR;

	float STROKE_WIDTH = DEFAULT_STROKE_WIDTH;

	void setStrokeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
		STROKE_COLOR.r = r;
		STROKE_COLOR.g = g;
		STROKE_COLOR.b = b;
		STROKE_COLOR.a = a;
	}

	void setStrokeColor(ofColor color) {
		STROKE_COLOR = color;
	}

	void setFilledColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
		FILLED_COLOR.r = r;
		FILLED_COLOR.g = g;
		FILLED_COLOR.b = b;
		FILLED_COLOR.a = a;
	}

	void setFilledColor(ofColor color) {
		FILLED_COLOR = color;
	}
};

} // namespace ift3100

#endif