#ifndef IFT_3100_PARAM_CURVE_H
#define IFT_3100_PARAM_CURVE_H

#include "ofMain.h"
#include <vector>

namespace ift3100 {
enum ParamCurveType { Bezier, Hermite };

class ParamCurve {
private:
public:
	/**
	 * @param t type of curve
	 * @param _precision amount of generated vertices, default as 50
	*/
	ParamCurve(ParamCurveType t, int _precision = 50);

	/**
	 * Set proper data for curve computation
	 * @param pos position of center of the curve
	 * @param p1 control point number 1
	 * @param p2 control point number 2
	 * @param p3 control point number 3
	 * @param p4 control point number 4
	 *
	*/
	void setup(ofNode _pos, ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4);
	void draw();

	/**
	 * Compute line points from control points
	*/
	void update();

	ofPolyline line; // Rendered line

	std::vector<ofVec3f> points;		// Control points
	std::vector<ofVec3f> cached_points; // Cache to reduce update calls

	ParamCurveType type;	   // Type of the curve
	ofNode		   pos;		   // Offset/Position of the curve
	ofNode		   cached_pos; // Cache to reduce update calls
	int			   precision;  // Amount of generated vertices
};
} // namespace ift3100

#endif