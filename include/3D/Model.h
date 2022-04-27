#ifndef MODEL_H
#define MODEL_H

#include "of3dPrimitives.h"
#include "ofTexture.h"
#include "ofMesh.h"
#include "ofNode.h"
#include "Logger.h"

namespace ift3100 {

enum ShaderType { NoShader, SobelFilter, GrayScale, Gaussian };

/**
 * @brief Class for imported models in order to map texture on them
 */
class Model {
	static ofShader _sobelShader;
	static ofShader _grayScaleShader;
	static ofShader _gaussianShader;

	ofTexture	  _texture;
	of3dPrimitive _primitive;
	ofMaterial	  material;

public:
	static void setup();

	ShaderType usingShader;

	Model(const Model& model);
	Model(of3dPrimitive primitive);
	Model(ofMesh mesh, ofTexture texture = ofTexture());

	void draw(bool isMaterialEnabled = true);
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
};

} // namespace ift3100

#endif