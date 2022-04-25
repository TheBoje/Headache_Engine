#include "ParamCurve.h"
#include "Logger.h"

namespace ift3100 {
ParamCurve::ParamCurve(ParamCurveType t, int _precision)
	: type(t)
	, pos(ofNode())
	, precision(_precision) { }

void ParamCurve::setup(ofNode _pos, ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4) {
	pos			  = _pos;
	cached_pos	  = _pos;
	points		  = {p1, p2, p3, p4};
	cached_points = {}; // Force first iteration computation on update
	for (int i = 0; i <= precision; i++) {
		line.addVertex(ofPoint());
	}
}

void ParamCurve::update() {
	// If any of the attributes changes, compute the curve points again, else do nothing
	if (pos.getGlobalTransformMatrix() == cached_pos.getGlobalTransformMatrix() && points == cached_points) {
		return;
	}
	cached_pos	  = pos; // If changed, update cached data
	cached_points = points;
	// Compute points
	switch (type) {
		case Bezier:
			for (int i = 0; i <= precision; i++) {
				float t = i / (float)precision;

				float x = pow(1 - t, 3) * points.at(0).x + 3 * pow(1 - t, 2) * t * points.at(1).x + 3 * (1 - t) * pow(t, 2) * points.at(2).x +
						  pow(t, 3) * points.at(3).x;
				float y = pow(1 - t, 3) * points.at(0).y + 3 * pow(1 - t, 2) * t * points.at(1).y + 3 * (1 - t) * pow(t, 2) * points.at(2).y +
						  pow(t, 3) * points.at(3).y;
				float z = pow(1 - t, 3) * points.at(0).z + 3 * pow(1 - t, 2) * t * points.at(1).z + 3 * (1 - t) * pow(t, 2) * points.at(2).z +
						  pow(t, 3) * points.at(3).z;
				line[i] = {x, y, z};
			}
			break;
		case Hermite:
			ofVec3f v1 = points.at(1) - points.at(0);
			ofVec3f v2 = points.at(2) - points.at(3);
			for (int i = 0; i <= precision; i++) {
				float t	  = i / (float)precision;
				float u	  = 1 - t;
				float uu  = u * u;
				float uuu = uu * u;
				float tt  = t * t;
				float ttt = tt * t;

				float x =
					(2 * ttt - 3 * tt + 1) * points.at(0).x + (ttt - 2 * tt + t) * v1.x + (ttt - tt) * v2.x + (-2 * ttt + 3 * tt) * points.at(3).x;
				float y =
					(2 * ttt - 3 * tt + 1) * points.at(0).y + (ttt - 2 * tt + t) * v1.y + (ttt - tt) * v2.y + (-2 * ttt + 3 * tt) * points.at(3).y;
				float z =
					(2 * ttt - 3 * tt + 1) * points.at(0).z + (ttt - 2 * tt + t) * v1.z + (ttt - tt) * v2.z + (-2 * ttt + 3 * tt) * points.at(3).z;
				line[i] = {x, y, z};
			}
			break;
	}
}

void ParamCurve::draw() {
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofSetLineWidth(50);
	ofPushMatrix();
	ofTranslate(pos.getPosition());
	ofVec3f rot = pos.getOrientationEulerDeg();
	ofRotate(rot.x, 1, 0, 0);
	ofRotate(rot.y, 0, 1, 0);
	ofRotate(rot.z, 0, 0, 1);
	ofScale(pos.getScale());
	line.draw();
	ofPopMatrix();
	// pos.draw();
}

} // namespace ift3100