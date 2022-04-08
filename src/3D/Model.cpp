#include "Model.h"
#include "ofImage.h"

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

void Model::loadTexture(std::string path) {
	ofImage image;
	image.load(path);
	_texture = image.getTexture();
}
} // namespace ift3100