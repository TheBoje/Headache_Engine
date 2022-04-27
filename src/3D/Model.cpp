#include "Model.h"
#include "ofImage.h"

namespace ift3100 {

ofShader Model::_sobelShader;
ofShader Model::_grayScaleShader;
ofShader Model::_gaussianShader;
ofShader Model::_toneMappingShader;

/**
 * @brief Setup the static shaders in order to store
 * only one instance for all of the models
 */
void Model::setup() {
	_sobelShader.load("../../src/Shaders/Filters/Sobel/sobel.vert.glsl", "../../src/Shaders/Filters/Sobel/sobel.frag.glsl");
	_grayScaleShader.load("../../src/Shaders/Filters/GrayScale/grayscale.vert.glsl", "../../src/Shaders/Filters/GrayScale/grayscale.frag.glsl");
	_gaussianShader.load("../../src/Shaders/Filters/Gaussian/gaussian.vert.glsl", "../../src/Shaders/Filters/Gaussian/gaussian.frag.glsl");
	_toneMappingShader.load(
		"../../src/Shaders/Filters/ToneMapping/tonemapping.vert.glsl", "../../src/Shaders/Filters/ToneMapping/tonemapping.frag.glsl");
}

Model::Model(const Model& model)
	: _texture(model._texture)
	, _primitive(model._primitive)
	, usingShader(model.usingShader) { }

Model::Model(of3dPrimitive primitive)
	: _texture(ofTexture())
	, _primitive(primitive)
	, usingShader(ShaderType::NoShader) {
	_material.setShininess(120);

	_material.setSpecularColor(ofColor(255, 255, 255, 255));
	_material.setEmissiveColor(ofColor(0, 0, 0, 255));
	_material.setDiffuseColor(ofColor(255, 255, 255, 255));
	_material.setAmbientColor(ofColor(255, 255, 255, 255));
}

Model::Model(ofMesh mesh, ofTexture texture)
	: _texture(texture)
	, usingShader(ShaderType::NoShader) {
	_primitive.getMesh() = mesh;

	_material.setShininess(120);

	_material.setSpecularColor(ofColor(255, 255, 255, 255));
	_material.setEmissiveColor(ofColor(0, 0, 0, 255));
	_material.setDiffuseColor(ofColor(255, 255, 255, 255));
	_material.setAmbientColor(ofColor(255, 255, 255, 255));
}

/**
 * @brief Draw the 3D model,
 * Apply a filter if a texture is set and one is applied
 * via the inspectorInterface
 * @link InspectorInterface @endlink
 */
void Model::draw(bool isMaterialEnabled) {
	if (isMaterialEnabled)
		_material.begin();

	if (_texture.isAllocated()) {
		_texture.bind();
		switch (usingShader) {
			case ShaderType::NoShader: _primitive.draw(); break;

			case ShaderType::SobelFilter:
				_sobelShader.begin();
				_sobelShader.setUniform2f("texSize", _texture.getWidth(), _texture.getHeight());
				_sobelShader.setUniform1f("threshold", sobelThreshold);
				_primitive.draw();
				_sobelShader.end();
				break;

			case ShaderType::GrayScale:
				_grayScaleShader.begin();
				_primitive.draw();
				_grayScaleShader.end();
				break;

			case ShaderType::Gaussian:
				_gaussianShader.begin();
				_gaussianShader.setUniform1f("blurAmnt", blurAmnt);
				_gaussianShader.setUniform2f("texSize", _texture.getWidth(), _texture.getHeight());
				_primitive.draw();
				_gaussianShader.end();
				break;

			case ShaderType::ToneMapping:
				_toneMappingShader.begin();
				_toneMappingShader.setUniform1f("tone_mapping_exposure", toneMappingExposure);
				_toneMappingShader.setUniform1f("tone_mapping_gamma", toneMappingGamma);
				_toneMappingShader.setUniform1i("tone_mapping_toggle", toggleToneMapping);
				_primitive.draw();
				_toneMappingShader.end();
		}
		_texture.unbind();
	} else {
		_primitive.draw();
	}

	if (isMaterialEnabled)
		_material.end();
}

/**
 * @brief Load an image and set it as a
 * texture for the 3D model.
 *
 * @param path relative path to the image from the bin/data folder
 */
void Model::loadTexture(std::string path) {
	ofImage image;
	image.load(path);
	_texture = image.getTexture();
}

void Model::setMesh(const ofMesh& mesh) {
	_primitive.getMesh() = ofMesh(mesh);
}

} // namespace ift3100