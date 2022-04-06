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
	, _type(ObjectType::Primitive)
	, _primitive(new of3dPrimitive(primitive)) { }

Object3D::Object3D(std::string name, ofMesh mesh)
	: _name(name)
	, _type(ObjectType::Primitive)
	, _primitive(new of3dPrimitive()) {
	_primitive->getMesh() = mesh;
}

Object3D::Object3D(std::string name, ofNode node)
	: _name(name)
	, _type(ObjectType::Node)
	, _node(new ofNode(node)) { }

Object3D::Object3D(std::string name, ofLight node)
	: _name(name)
	, _type(ObjectType::Light)
	, _light(new ofLight(node)) { }

Object3D::~Object3D() {
	delete getNode();
}

ofNode* Object3D::getNode() {
	switch (_type) {
		case ObjectType::Camera: return _camera;
		case ObjectType::Primitive: return _primitive;
		case ObjectType::Node: return _node;
		case ObjectType::Light: return _light;
		default: IFT_LOG_ERROR << "try getting node of " << _type << ", returning nullptr instead"; return nullptr;
	}
}

} // namespace ift3100