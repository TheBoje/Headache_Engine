#include "Model.h"
#include "ofImage.h"

namespace ift3100 {

Model::Model(of3dPrimitive primitive)
	: _texture(ofTexture())
	, _primitive(primitive)
	, usingShader(ShaderType::NoShader) {
	_sobelShader.load("../../src/Shaders/Filters/Sobel/sobel.vert.glsl", "../../src/Shaders/Filters/Sobel/sobel.frag.glsl");
}

Model::Model(ofMesh mesh, ofTexture texture)
	: _texture(texture)
	, usingShader(ShaderType::NoShader) {
	_primitive.getMesh() = mesh;
	_primitive.mapTexCoordsFromTexture(_texture);
	_sobelShader.load("../../src/Shaders/Filters/Sobel/sobel.vert.glsl", "../../src/Shaders/Filters/Sobel/sobel.frag.glsl");
}

void Model::draw() {
	if (_texture.isAllocated()) {
		_texture.bind();

		switch (usingShader) {
			case ShaderType::NoShader: _primitive.draw(); break;

			case ShaderType::SobelFilter:
				_sobelShader.begin();
				_sobelShader.setUniform2f("texSize", _texture.getWidth(), _texture.getHeight());
				_primitive.draw();
				_sobelShader.end();
		}

		_texture.unbind();
	} else {
		_primitive.draw();
	}
}

void Model::loadTexture(std::string path) {
	ofImage image;
	image.load(path);
	_texture = image.getTextureReference();
	_primitive.mapTexCoordsFromTexture(_texture);
}
} // namespace ift3100