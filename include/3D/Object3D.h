#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "HierarchyItem.h"

#include "of3dPrimitives.h"
#include "ofCamera.h"
#include "ofMesh.h"
#include "Model.h"

#include <string>

namespace ift3100 {
enum ObjectType { Camera, Node, Light, Model3D, NoneObj };

class Object3D : public HierarchyItem {
	std::string _name;
	ObjectType	_type;
	ofTexture	test;

	union {
		ofCamera* _camera;
		Model*	  _model;
		ofNode*	  _node;
		ofLight*  _light;
	};

public:
	Object3D(std::string name);
	Object3D(std::string name, ofCamera camera);
	Object3D(std::string name, of3dPrimitive primitive);
	Object3D(std::string name, ofMesh mesh, ofTexture texture = ofTexture());
	Object3D(std::string name, ofNode node);
	Object3D(std::string name, ofLight node);
	~Object3D();

	ofNode* getNode();

	void draw(bool isSelected = false);

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