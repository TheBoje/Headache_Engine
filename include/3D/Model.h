#ifndef MODEL_H
#define MODEL_H

#include "Logger.h"
#include "of3dPrimitives.h"
#include "ofMesh.h"
#include "ofNode.h"
#include "ofTexture.h"

namespace ift3100 {

enum ShaderType { NoShader, SobelFilter, GrayScale, Gaussian, ToneMapping };

/**
 * @brief Class for imported models in order to map texture on them
 */
class Model {
	static ofShader _sobelShader;
	static ofShader _grayScaleShader;
	static ofShader _gaussianShader;
	static ofShader _toneMappingShader;

	ofTexture _texture;
	of3dPrimitive _primitive;
	ofMaterial _material;

public:
	static void setup();

	float transparency = 0;
	float reflection = 0;

	float sobelThreshold = 2.0f;
	float blurAmnt = 4.0f;

	float toneMappingExposure = 1.0f;
	float toneMappingGamma = 2.2f;
	bool toggleToneMapping = true; // true -> aces filming, false -> Reihnard
	bool enableMaterial = true;

	ShaderType usingShader;

	Model(const Model& model);
	Model(of3dPrimitive primitive);
	Model(ofMesh mesh, ofTexture texture = ofTexture());

	void draw();
	void loadTexture(std::string path);

	void setMesh(const ofMesh& mesh);

	inline ofMesh* getMesh() {
		return _primitive.getMeshPtr();
	}

	inline ofNode* getNode() {
		return &_primitive;
	}

	inline ofTexture* getTexture() {
		return &_texture;
	}

	inline void setTexture(ofTexture texture) {
		_texture = texture;
		_primitive.mapTexCoordsFromTexture(_texture);
	}

	inline of3dPrimitive& getPrimitive() {
		return _primitive;
	}

	inline ofMaterial& getMaterial() {
		return _material;
	}

	inline void setMaterial(const ofMaterial& mat) {
		_material = mat;
	}
};

} // namespace ift3100

#endif