#include "ParamCurve.h"
#include "Asserts.h"
#include "Logger.h"

namespace ift3100 {
ParamCurve::ParamCurve(ParamCurveType t, int _precision)
	: type(t)
	, pos(ofNode())
	, precision(_precision) { }

void ParamCurve::setup(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4) {
	cached_pos = pos;
	points = {p1, p2, p3, p4};
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
	// If changed, update cached data
	cached_pos = pos;
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
				float t = i / (float)precision;

				float x = (2 * pow(t, 3) - 3 * (t * t) + 1) * points.at(0).x + (pow(t, 3) - 2 * pow(t, 2) + t) * v1.x +
						  (pow(t, 3) - pow(t, 2)) * v2.x + (-2 * pow(t, 3) + 3 * pow(t, 2)) * points.at(3).x;
				float y = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * points.at(0).y + (pow(t, 3) - 2 * pow(t, 2) + t) * v1.y +
						  (pow(t, 3) - pow(t, 2)) * v2.y + (-2 * pow(t, 3) + 3 * pow(t, 2)) * points.at(3).y;
				float z = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * points.at(0).z + (pow(t, 2) - 2 * pow(t, 2) + t) * v1.z +
						  (pow(t, 3) - pow(t, 2)) * v2.z + (-2 * pow(t, 3) + 3 * pow(t, 2)) * points.at(3).z;
				line[i] = {x, y, z};
			}
			break;
	}
}

void ParamCurve::draw() {
	ofFill();
	ofSetColor(255, 255, 255);
	ofSetLineWidth(50);
	ofPushMatrix();
	ofTranslate(pos.getPosition());
	ofVec3f rot = pos.getOrientationEulerDeg();
	ofRotateDeg(rot.x, 1, 0, 0);
	ofRotateDeg(rot.y, 0, 1, 0);
	ofRotateDeg(rot.z, 0, 0, 1);
	ofScale(pos.getScale());
	line.draw();
	ofPopMatrix();
}

ofVec3f ParamCurve::getValue(double i) {
	update(); // Maybe the line is outdated if not init?
	return line.getPointAtPercent(i);
}

} // namespace ift3100