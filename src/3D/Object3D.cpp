#include "Object3D.h"
#include "Asserts.h"
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

Object3D::Object3D(std::string name, ParamCurve curve)
	: _name(name)
	, _type(ObjectType::ParametricCurve)
	, _curve(new ParamCurve(curve)) { }

Object3D::Object3D(std::string name, ParamSurface surface)
	: _name(name)
	, _type(ObjectType::ParametricSurface)
	, _surface(new ParamSurface(surface)) { }

Object3D::Object3D(std::string name, Voronoi3D voronoi)
	: _name(name)
	, _type(ObjectType::Voronoi)
	, _voronoi3D(new Voronoi3D(voronoi)) { }

Object3D::~Object3D() {
	switch (_type) {
		case ObjectType::Model3D: delete _model; break;
		case ObjectType::ParametricCurve: delete _curve; break;
		case ObjectType::ParametricSurface: delete _surface; break;
		case ObjectType::Voronoi: delete _voronoi3D; break;
		default: delete getNode(); break;
	}
}

Model* Object3D::getModel() {
	IFT_ASSERT(_type == ObjectType::Model3D, "Get model is forbidden for non-Model3D objects. Try get getType() before.");
	return _model;
}

ParamCurve* Object3D::getCurve() {
	IFT_ASSERT(_type == ObjectType::ParametricCurve, "Get curve is forbidden for non-ParamCurve objects. Try get getType() before.");
	return _curve;
}

ParamSurface* Object3D::getSurface() {
	IFT_ASSERT(_type == ObjectType::ParametricSurface, "Get surface is forbidden for non-ParamSurface objects. Try get getType() before.");
	return _surface;
}

Voronoi3D* Object3D::getVoronoi3D() {
	IFT_ASSERT(_type == ObjectType::Voronoi, "Get surface is forbidden for non-Voronoi3D objects. Try get getType() before.");
	return _voronoi3D;
}

void Object3D::draw(bool isSelected) {
	switch (_type) {
		case ObjectType::Model3D: _model->draw(); break;
		case ObjectType::ParametricCurve: _curve->draw(); break;
		case ObjectType::ParametricSurface: _surface->draw(); break;
		case ObjectType::Voronoi: _voronoi3D->draw(); break;
		case ObjectType::Node:
			if (isSelected)
				getNode()->draw();
		default: getNode()->draw(); break;
	}
}

void Object3D::update() {
	switch (_type) {
		case ObjectType::ParametricCurve: _curve->update(); break;
		case ObjectType::ParametricSurface: _surface->update(); break;
		case ObjectType::Voronoi: _voronoi3D->update(); break;
		default: break;
	}
}

ofNode* Object3D::getNode() {
	switch (_type) {
		case ObjectType::Camera: return _camera;
		case ObjectType::Model3D: return _model->getNode();
		case ObjectType::Node: return _node;
		case ObjectType::Light: return _light;
		case ObjectType::ParametricCurve: return &_curve->pos;
		case ObjectType::ParametricSurface: return &_surface->pos;
		case ObjectType::Voronoi: return &_voronoi3D->pos;
		default: IFT_LOG_ERROR << "try getting node of " << _type << ", returning nullptr instead"; return nullptr;
	}
}

} // namespace ift3100