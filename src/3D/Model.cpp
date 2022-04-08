#include "Model.h"

namespace ift3100 {

Model::Model(of3dPrimitive primitive)
	: _texture(ofTexture())
	, _primitive(primitive) { }

Model::Model(ofMesh mesh, ofTexture texture)
	: _texture(texture) {
	_primitive.getMesh() = mesh;
}

void Model::draw() {
	if (_texture.isAllocated()) {
		_texture.bind();
		_primitive.drawFaces();
		_texture.unbind();
	} else {
		_primitive.drawFaces();
	}
}

} // namespace ift3100