#include "Voronoi3D.h"
#include "Asserts.h"
#include "Logger.h"
#include <voro++/voro++.hh>

namespace ift3100 {
void Voronoi3D::setup(std::vector<ofVec3f> _points, ofBoxPrimitive _bounds) {
	bounds = _bounds;
	points = _points;

	compute();
}

void Voronoi3D::update() {
	// TODO: check if input points changed, if so update diagram w/ compute(points).
}

void Voronoi3D::draw() {
	ofPushMatrix();
	ofMultMatrix(pos.getGlobalTransformMatrix()); // Offset draw to `pos` global matrix

	// Draw cells
	ofSetLineWidth(1);
	ofFill();
	for (int i = 0; i < points.size(); i++) {
		ofSetColor(0, 255, 0);
		ofDrawSphere(points.at(i), 2);
	}

	// Draw cell borders
	for (int i = 0; i < cells.size(); i++) {
		ofFill();
		ofSetColor(220, 220, 220, 30);
		// Cell borders
		cells.at(i).draw();
		ofNoFill();
		ofSetLineWidth(15);
		ofSetColor(255, 255, 255, 90);
		// Cell wireframe
		cellsWireframe.at(i).draw();
	}
	ofPopStyle();
	ofPopMatrix();
}

// compute() helper
vector<glm::vec3> getCellVertices(voro::voronoicell& _c, ofPoint _pos) {
	double* ptsp = _c.pts;
	vector<glm::vec3> vertices;
	for (int i = 0; i < _c.p; i++, ptsp += 3) {
		glm::vec3 newPoint;
		newPoint[0] = _pos.x + ptsp[0] * 0.5;
		newPoint[1] = _pos.y + ptsp[1] * 0.5;
		newPoint[2] = _pos.z + ptsp[2] * 0.5;
		vertices.push_back(newPoint);
	}
	return vertices;
}

// compute() helper
void getCellMesh(voro::voronoicell& _c, ofPoint _pos, ofMesh& _mesh, bool bWireframe) {
	if (_c.p) {
		if (!bWireframe) {
			//  Extract Verteces
			ofVboMesh mesh;
			mesh.setMode(OF_PRIMITIVE_TRIANGLES);
			mesh.addVertices(getCellVertices(_c, _pos));

			//  Add triangles using Indeces
			// NOTE(Louis): please make this easier to read, this is black magic now
			int k, l, m, n;
			for (int i = 1; i < _c.p; i++) {
				for (int j = 0; j < _c.nu[i]; j++) {
					k = _c.ed[i][j];
					if (k >= 0) {
						_c.ed[i][j] = -1 - k;
						l = _c.cycle_up(_c.ed[i][_c.nu[i] + j], k);
						m = _c.ed[k][l];
						_c.ed[k][l] = -1 - m;
						while (m != i) {
							n = _c.cycle_up(_c.ed[k][_c.nu[k] + l], m);
							mesh.addTriangle(i, k, m);
							k = m;
							l = n;
							m = _c.ed[k][l];
							_c.ed[k][l] = -1 - m;
						}
					}
				}
			}

			//  Calculate Normals
			_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
			vector<ofMeshFace> faces = mesh.getUniqueFaces();
			for (int i = 0; i < faces.size(); i++) {
				ofMeshFace face = faces[i];
				ofPoint a = face.getVertex(0);
				ofPoint b = face.getVertex(1);
				ofPoint c = face.getVertex(2);

				ofPoint normal = ((b - a).cross(c - a)).normalize() * -1.;

				_mesh.addVertex(a);
				_mesh.addNormal(normal);

				_mesh.addVertex(b);
				_mesh.addNormal(normal);
				_mesh.addVertex(c);
				_mesh.addNormal(normal);

				// add texture coordinates
				if (i % 2 == 0) {
					_mesh.addTexCoord(ofVec2f(0.0, 0.0));
					_mesh.addTexCoord(ofVec2f(0.0, 1.0));
					_mesh.addTexCoord(ofVec2f(1.0, 0.0));
				} else {
					_mesh.addTexCoord(ofVec2f(1.0, 0.0));
					_mesh.addTexCoord(ofVec2f(0.0, 1.0));
					_mesh.addTexCoord(ofVec2f(1.0, 1.0));
				}
			}
		} else {
			// Wireframe
			_mesh.setMode(OF_PRIMITIVE_LINES);
			_mesh.addVertices(getCellVertices(_c, _pos));
			for (int i = 1; i < _c.p; i++) {
				for (int j = 0; j < _c.nu[i]; j++) {
					int k = _c.ed[i][j];
					if (k >= 0) {
						_mesh.addIndex(i);
						_mesh.addIndex(k);
					}
				}
			}
		}
	}
}

// compute() helper
std::vector<ofMesh> getCells(voro::container& _con, bool bWireframe) {
	vector<ofMesh> _cells = {};
	voro::c_loop_all vl(_con);
	int i = 0;
	if (vl.start()) {
		do {
			voro::voronoicell c;
			if (!_con.compute_cell(c, vl)) {
				return _cells;
			} else {
				double* pp = _con.p[vl.ijk] + _con.ps * vl.q;
				ofMesh cellMesh;
				cellMesh.clear();
				getCellMesh(c, ofPoint(pp[0], pp[1], pp[2]), cellMesh, bWireframe);
				_cells.push_back(cellMesh);
				i++;
			}
		} while (vl.inc());
	}
	return _cells;
}

void Voronoi3D::compute() {
	IFT_ASSERT(points.size() >= 2, "Voronoi diagram needs at least 2 input points");
	voro::container* con = new voro::container(bounds.getPosition().x - (bounds.getWidth() / 2),
		bounds.getPosition().x + (bounds.getWidth() / 2),
		bounds.getPosition().y - (bounds.getHeight() / 2),
		bounds.getPosition().y + (bounds.getHeight() / 2),
		bounds.getPosition().z - (bounds.getDepth() / 2),
		bounds.getPosition().z + (bounds.getDepth() / 2),
		1,
		1,
		1,
		false,
		false,
		false,
		16);

	for (int i = 0; i < points.size(); i++) {
		con->put(i, points[i].x, points[i].y, points[i].z);
	}
	cells.clear();
	cells = getCells(*con, false);
	cellsWireframe.clear();
	cellsWireframe = getCells(*con, true);

	delete con;
	IFT_LOG << "updated points";
}
} // namespace ift3100