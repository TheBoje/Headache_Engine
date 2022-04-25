#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "HierarchyItem.h"

#include "Model.h"
#include "ParamCurve.h"
#include "of3dPrimitives.h"
#include "ofCamera.h"
#include "ofMesh.h"

#include <string>

namespace ift3100 {
enum ObjectType { Camera, Node, Light, Model3D, ParametricCurve, NoneObj };

class Object3D : public HierarchyItem {
	std::string _name;
	ObjectType	_type;
	ofTexture	test;

	union {
		ofCamera*	_camera;
		Model*		_model;
		ofNode*		_node;
		ofLight*	_light;
		ParamCurve* _curve;
	};

public:
	Object3D(std::string name);
	Object3D(std::string name, ofCamera camera);
	Object3D(std::string name, of3dPrimitive primitive);
	Object3D(std::string name, ofMesh mesh, ofTexture texture = ofTexture());
	Object3D(std::string name, ofNode node);
	Object3D(std::string name, ofLight node);
	Object3D(std::string name, ParamCurve curve);
	~Object3D();

	ofNode*		getNode();
	Model*		getModel();
	ParamCurve* getCurve();

	void draw(bool isSelected = false);
	void update();

	ObjectType getType() const {
		return _type;
	}
	std::string toString() const {
		return _name;
	}
	void setName(std::string name) {
		_name = name;
	}
};

} // namespace ift3100

#endif