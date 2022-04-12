#include "Object3D.h"
#include "Logger.h"

namespace ift3100 {
Object3D::Object3D(std::string name)
	: _name(name)
	, _type(ObjectType::NoneObj) { }

Object3D::Object3D(std::string name, ofCamera camera)
	: _name(name)
	, _type(ObjectType::Camera)
	, _camera(new ofCamera(camera)) { }

Object3D::Object3D(std::string name, of3dPrimitive primitive)
	: _name(name)
	, _type(ObjectType::Model3D)
	, _model(new Model(primitive)) { }

Object3D::Object3D(std::string name, ofMesh mesh, ofTexture texture)
	: _name(name)
	, _type(ObjectType::Model3D)
	, _model(new Model(mesh, texture)) { }

Object3D::Object3D(std::string name, ofNode node)
	: _name(name)
	, _type(ObjectType::Node)
	, _node(new ofNode(node)) { }

Object3D::Object3D(std::string name, ofLight node)
	: _name(name)
	, _type(ObjectType::Light)
	, _light(new ofLight(node)) { }

Object3D::~Object3D() {
	// delete getNode(); TODO: delete allocated (throw seg fault if deleted)
}

Model* Object3D::getModel() {
	assert(_type == ObjectType::Model3D);
	return _model;
}

void Object3D::draw(bool isSelected) {
	if (_type == ObjectType::Model3D)
		_model->draw();
	else if (ObjectType::Node) {
		if (isSelected)
			getNode()->draw();
	} else
		getNode()->draw();
}

ofNode* Object3D::getNode() {
	switch (_type) {
		case ObjectType::Camera: return _camera;
		case ObjectType::Model3D: return _model->getNode();
		case ObjectType::Node: return _node;
		case ObjectType::Light: return _light;
		default: IFT_LOG_ERROR << "try getting node of " << _type << ", returning nullptr instead"; return nullptr;
	}
}

} // namespace ift3100