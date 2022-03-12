#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include "HierarchyItem.h"

#include "ofCamera.h"
#include "of3dPrimitives.h"
#include "ofMesh.h"

#include <string>

namespace ift3100
{
    enum ObjectType {
        Camera,
        Primitive,
        Mesh,
        Node,
        NoneObj
    };


    class Object3D : public HierarchyItem {
        std::string _name;
        ObjectType  _type;

        union {
            ofCamera *        _camera;
            of3dPrimitive *   _primitive;
            ofMesh *          _mesh;
            ofNode *          _node;
        };

    public:
        Object3D(std::string name);
        Object3D(std::string name, ofCamera camera);
        Object3D(std::string name, of3dPrimitive primitive);
        Object3D(std::string name, ofMesh mesh);
        Object3D(std::string name, ofNode node);
        ~Object3D();

        ofNode* getNode();

        ObjectType getType() const { return _type; }
        std::string toString() const { return _name; }
        void setName(std::string name) { _name = name; }
    };

} // namespace ift3100


#endif