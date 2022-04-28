#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "ofCamera.h"
#include "ofLight.h"
#include "ofImage.h"

#include "Model.h"
#include "Ray.h"

namespace ift3100 {

class Raytracing {
	// TODO: voir pour le paralléliser

	ofCamera*			  _viewSource;
	std::vector<ofLight*> _lights;
	std::vector<Model*>	  _objects;

	ofImage _result;

public:
	Raytracing(ofCamera* src, std::vector<ofLight*>& lights, std::vector<Model*>& objs);

	void render();
};

} // namespace ift3100

#endif