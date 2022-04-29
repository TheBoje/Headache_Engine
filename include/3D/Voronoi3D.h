#ifndef IFT3100_VORONOI_3D_H
#define IFT3100_VORONOI_3D_H

#include "ofMain.h"
#include <vector>

namespace ift3100 {
class Voronoi3D {
private:
	/**
	 * Compute voronoi diagram from `_points` & `_bounds`, and set `cells` and `cellsWireframe` accordingly.
	 * This computation is very intensive, PLEASE DO NOT CALL IT EVERY FRAME!
	 * For now, its only called on setup, as internal data is not yet updated.
	*/
	void compute();

public:
	/**
	 * @param _points list of control points for voronoi diagram
	 * @param _bounds bounds of the calculation
	*/
	void setup(std::vector<ofVec3f> _points, ofBoxPrimitive _bounds);
	void update();
	/**
	 * Draw transparent cells, with their wireframes, and the control points.
	*/
	void draw();

	ofNode pos;							// Global position
	ofBoxPrimitive bounds;				// Bounds of the voronoi calculation. Diagram is only computed within it
	std::vector<ofMesh> cells;			// Meshs of every individual cells
	std::vector<ofMesh> cellsWireframe; // Meshs of every cells' wireframe
	std::vector<ofVec3f> points;		// List of inputs points (from which voronoi diagram is computed)
};
} // namespace ift3100
#endif //!IFT3100_VORONOI_3D_H