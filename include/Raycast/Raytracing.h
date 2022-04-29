#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "ofCamera.h"
#include "ofLight.h"
#include "ofImage.h"

#include "Model.h"
#include "Ray.h"

namespace ift3100 {

/**
 * @brief Ray tracing by Monte Carlo approche
 * 
 */
class Raytracing {
	// TODO: voir pour le paralléliser

	const int MAX_DEPTH = 3;

	ofCamera*			  _viewSource;
	std::vector<ofLight*> _lights;
	std::vector<Model*>	  _objects;

	ofImage _result;

	void getIntersectionInWorld(Ray& ray, int* indexHitobj, int* indexHitlight, Intersection* inter);

	ofColor lightspath(const Intersection& inter, int depth);
	ofColor tracepath(Ray& ray, int depth);

public:
	Raytracing(ofCamera* src, std::vector<ofLight*>& lights, std::vector<Model*>& objs);

	void render();
	void saveImage();
};

} // namespace ift3100

#endif