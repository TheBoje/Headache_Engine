#ifndef RAY_H
	#define RAY

	#include "ofVec3f.h"
	#include "of3dPrimitives.h"

namespace ift3100 {

typedef struct inter {
	bool	intersect;
	ofVec3f position;
} Intersection;

class Ray {
	// TODO: define a payload for many puposes ? or maybe a template

	ofVec3f _origin;
	ofVec3f _direction; // normalized vector
	float	_length;

	void init(ofVec3f src, ofVec3f dir, float length = -1);

public:
	Ray(ofVec3f src, ofVec3f dir, float length = -1);
	Ray(ofVec3f dir);

	void draw();

	Intersection intersect(of3dPrimitive obj);

	/**
     * @brief Set the direction of the Ray to dir.normalized()
     * 
     * @param dir 
     */
	inline void setDirection(ofVec3f dir) {
		_direction = dir.normalized();
	}
	inline void setOrigin(ofVec3f src) {
		_origin = src;
	}
	inline void setLegth(float length) {
		_length = length;
	}
};

} // namespace ift3100

#endif