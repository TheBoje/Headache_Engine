#ifndef RAY_H
	#define RAY

	#include "ofVec3f.h"
	#include "of3dPrimitives.h"

namespace ift3100 {

/**
 * @brief structure storing :
 * intersect: bool - evaluate if there is an intersection
 * position: ofVec3f - the position of the intersection in the global world
 * normal: ofVec3f - the normal of the triangle intersected
 */
typedef struct inter {
	bool	intersect;
	ofVec3f position;
	ofVec3f normal;
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
	Ray			 reflect(Intersection inter);
	Ray			 refract(Intersection inter, float n1, float n2);

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