#ifndef IFT_3100_PARAM_SURFACE_H
#define IFT_3100_PARAM_SURFACE_H

#include "ofMain.h"

namespace ift3100 {
enum ParamSurfaceType { Coons };

class ParamSurface {
public:
	ParamSurface(ParamSurfaceType t, int _precisionX, int _precisionY);
	void setup(std::vector<ofVec3f> _points);
	void update();
	void draw();
	void loadTexture(std::string path);

	ofMesh mesh;
	ParamSurfaceType type;

	std::vector<ofVec3f> points, cached_points;
	ofNode pos, cached_pos;
	int precisionX, precisionY;
	ofTexture _texture;
};
} // namespace ift3100

#endif