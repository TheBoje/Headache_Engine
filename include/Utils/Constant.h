#ifndef CONSTANT_H
#define CONSTANT_H

#include "ofColor.h"

namespace ift3100 {
// FIXME: I can't seem to make those ofColor work for some reason.
const ofColor DEFAULT_BACKGROUND_COLOR = ofColor::darkGray;

const ofColor DEFAULT_STROKE_COLOR = ofColor::black;
const ofColor DEFAULT_FILL_COLOR = ofColor::white;

const float DEFAULT_STROKE_WIDTH = 1.0f;
const float DEFAULT_RECTANGLE_ROUNDING = 5.0f;

const int NB_AXES_CAM = 3;

const int CAMERA_WIDTH = 1024;

const int MAX_LIGHTS = 8;

} // namespace ift3100

#endif