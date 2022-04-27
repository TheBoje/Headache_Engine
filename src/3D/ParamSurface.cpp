#include "ParamSurface.h"
#include "Asserts.h"
#include "ParamCurve.h"

namespace ift3100 {

// compute n!
double factorial(int n) {
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

// For Coons patch equation
double lerpu(double u, double v, double c1u, double c2u) {
	return (1 - v) * c1u + v * c2u;
}

// For Coons patch equation
double lerpv(double u, double v, double c3v, double c4v) {
	return (1 - u) * c3v + u * c4v;
}

// For Coons patch equation
double blerp(double u, double v, double c11, double c12, double c21, double c22) {
	return (1 - u) * (1 - v) * c11 + u * (1 - v) * c12 + (1 - u) * v * c21 + u * v * c22;
}

ParamSurface::ParamSurface(ParamSurfaceType t, int _precisionX, int _precisionY)
	: type(t)
	, pos(ofNode())
	, precisionX(_precisionX)
	, precisionY(_precisionY) {
	mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES);
}

void ParamSurface::setup(std::vector<ofVec3f> _points) {
	points = _points;
}

void ParamSurface::update() {
	// If any of the attributes changes, compute the curve points again, else do nothing
	if (pos.getGlobalTransformMatrix() == cached_pos.getGlobalTransformMatrix() && points == cached_points) {
		return;
	}
	// If changed, update cached data
	cached_pos	  = pos;
	cached_points = points;
	switch (type) {
		case Coons:
			/*
			Expected input: Pi = input.at(i)
				P0 - P1 - P2 - P3
				|				|
				P11			   P4
				|				|
				P10			   P5
				|				|
				P9 - P8 - P7 - P6
			The points in between the corners are set as control points for parametric curves.
			*/
			IFT_ASSERT(points.size() >= 12, "Coons parametric curve require 12 control points.");
			// Coons path uses 4 curves, we use bezier for now.
			ParamCurve c1(ParamCurveType::Bezier, precisionX);
			c1.setup(points.at(0), points.at(1), points.at(2), points.at(3));
			ParamCurve c2(ParamCurveType::Bezier, precisionX);
			c2.setup(points.at(9), points.at(8), points.at(7), points.at(6));
			ParamCurve c3(ParamCurveType::Bezier, precisionY);
			c3.setup(points.at(0), points.at(11), points.at(10), points.at(9));
			ParamCurve c4(ParamCurveType::Bezier, precisionY);
			c4.setup(points.at(3), points.at(4), points.at(5), points.at(6));
			// Compute position of every points of the mesh following coons formula
			mesh.clear();
			for (int i = 0; i <= precisionX; i++) {
				double u = i / (double)precisionX;
				for (int j = 0; j <= precisionY; j++) {
					double v = j / (double)precisionY;
					double x = lerpu(u, v, c1.getValue(u).x, c2.getValue(u).x) + lerpv(u, v, c3.getValue(v).x, c4.getValue(v).x) +
							   blerp(u, v, points.at(0).x, points.at(3).x, points.at(9).x, points.at(6).x);
					double y = lerpu(u, v, c1.getValue(u).y, c2.getValue(u).y) + lerpv(u, v, c3.getValue(v).y, c4.getValue(v).y) +
							   blerp(u, v, points.at(0).y, points.at(3).y, points.at(9).y, points.at(6).y);
					double z = lerpu(u, v, c1.getValue(u).z, c2.getValue(u).z) + lerpv(u, v, c3.getValue(v).z, c4.getValue(v).z) +
							   blerp(u, v, points.at(0).z, points.at(3).z, points.at(9).z, points.at(6).z);
					mesh.addVertex({x, y, z});
				}
			}
			// Set index for mesh triangles
			for (int i = 1; i < precisionX; i++) {
				for (int j = 1; j < precisionY; j++) {
					mesh.addIndex(i * (precisionX + 1) + j);
					mesh.addIndex(i * (precisionX + 1) + j + 1);
					mesh.addIndex((i + 1) * (precisionX + 1) + j);
					mesh.addIndex(i * (precisionX + 1) + j + 1);
					mesh.addIndex((i + 1) * (precisionX + 1) + j + 1);
					mesh.addIndex((i + 1) * (precisionX + 1) + j);
				}
			}
			break;
	}
}

void ParamSurface::draw() {
	ofFill();
	ofPushMatrix();
	// Translate mesh by pos (ofNode) offset (position, rotation scale)
	ofTranslate(pos.getPosition());
	ofVec3f rot = pos.getOrientationEulerDeg();
	ofRotateDeg(rot.x, 1, 0, 0);
	ofRotateDeg(rot.y, 0, 1, 0);
	ofRotateDeg(rot.z, 0, 0, 1);
	ofScale(pos.getScale());
	// Draw with texture if possible
	if (_texture.isAllocated()) {
		_texture.bind();
		mesh.drawFaces();
		_texture.unbind();
	} else { // Only draw wireframe
		ofSetColor(255, 255, 255);
		ofSetLineWidth(50);
		mesh.drawWireframe();
	}
	// Stop translation
	ofPopMatrix();
}

/**
 * @brief Load an image and set it as a texture for the mesh.
 *
 * @param path relative path to the image from the bin/data folder
 */
void ParamSurface::loadTexture(std::string path) {
	ofImage image;
	image.load(path);
	_texture = image.getTexture();
}
} // namespace ift3100