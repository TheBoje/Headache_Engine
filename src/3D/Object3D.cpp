#include "Object3D.h"
#include "Logger.h"

namespace ift3100 {
    Object3D::Object3D(std::string name) :
        _name(name),
        _type(ObjectType::NoneObj)
    {}

    Object3D::Object3D(std::string name, ofCamera camera) :
        _name(name),
        _type(ObjectType::Camera),
        _camera(new ofCamera(camera))
    {}

    Object3D::Object3D(std::string name, of3dPrimitive primitive) :
        _name(name),
        _type(ObjectType::Primitive),
        _primitive(new of3dPrimitive(primitive))
    {}

    Object3D::Object3D(std::string name, ofMesh mesh) :
        _name(name),
        _type(ObjectType::Mesh),
        _mesh(new ofMesh(mesh))
    {}

    Object3D::Object3D(std::string name, ofNode node) :
        _name(name),
        _type(ObjectType::Node),
        _node(new ofNode(node))
    {}

    Object3D::~Object3D() {
        delete getNode();
    }

    ofNode* Object3D::getNode() {
        switch(_type) {
            case ObjectType::Camera:
                return _camera;
            case ObjectType::Primitive:
                return _primitive;
            case ObjectType::Node:
                return _node;
            default :
                IFT_LOG_ERROR << "try getting node of non ofNode object, returning nullptr instead";
                return nullptr;
        }
    }
}