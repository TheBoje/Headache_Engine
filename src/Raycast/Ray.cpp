#include "Ray.h"
#include "Asserts.h"

namespace ift3100 {

void Ray::init(ofVec3f src, ofVec3f dir, float length) {
	_origin	   = src;
	_direction = dir;
	_length	   = length;
}

/**
 * @brief Create a Ray object represented by it's source position,
 * it's direction and the length of the ray (= 10 by default).
 * The direction is normalised
 * 
 * @param src 
 * @param dir 
 * @param length 
 */
Ray::Ray(ofVec3f src, ofVec3f dir, float length) {
	IFT_ASSERT(length >= -1, "length cannot be < -1");
	init(src, dir.normalized(), length);
}

/**
 * @brief Create a Ray at the position 0.
 * The direction is normalised
 * 
 * @param dir 
 */
Ray::Ray(ofVec3f dir) {
	init(ofVec3f(0, 0, 0), dir.normalized());
}

void Ray::draw() {
	ofPushStyle();
	ofSetColor(255, 0, 0);
	ofDrawLine(_origin, _origin + _direction * (_length == -1 ? 1000 : _length));
	ofPopStyle();
}

Intersection Ray::intersect(of3dPrimitive obj) {
	Intersection inter;
	inter.intersect = false;
	inter.position	= ofVec3f(0, 0, 0);

	ofVec3f objPosition = obj.getPosition();
	ofMesh	mesh		= obj.getMesh();
	for (int i = 0; i < mesh.getVertices().size(); i++) {
		mesh.setVertex(i, obj.getGlobalTransformMatrix() * ofVec4f(mesh.getVertex(i).x, mesh.getVertex(i).y, mesh.getVertex(i).z, 1));
	}

	for (ofMeshFace face : mesh.getUniqueFaces()) {
		if (inter.intersect == true)
			break;

		// Check if the ray go through the plan
		ofVec3f normal = face.getFaceNormal() * -1;
		if (_direction.dot(normal) < 0) {
			ofVec3f normalPosition = (face.getVertex(0) + face.getVertex(1) + face.getVertex(2)) / 3;

			float t = ((normalPosition - _origin).dot(normal)) / _direction.dot(normal);

			ofVec3f q1 = face.getVertex(1) - face.getVertex(0);
			ofVec3f q2 = face.getVertex(2) - face.getVertex(0);
			ofVec3f r  = (_origin + _direction * t) - face.getVertex(0);

			// Louis forgive me i need local variables
			float q1s(q1.dot(q1)), q2s(q2.dot(q2));
			float q1q2(q1.dot(q2)), mq1q2((q1 * -1).dot(q2));

			float ratio = 1 / ((q1s * q2s) - (q1q2 * q1q2));

			float w1 = ratio * (q2s * r.dot(q1) + mq1q2 * r.dot(q2));
			float w2 = ratio * (mq1q2 * r.dot(q1) + q1s * r.dot(q2));
			float w0 = 1 - w1 - w2;

			if (w0 > 0 && w1 > 0 && w2 > 0 && (w1 + w2 <= 1)) {
				inter.position	= (_origin + _direction * t);
				inter.intersect = true;
			}
		}
	}

	return inter;
}

} // namespace ift3100