#ifndef MODEL_H
#define MODEL_H

#include "of3dPrimitives.h"
#include "ofTexture.h"
#include "ofMesh.h"
#include "ofNode.h"
#include "Logger.h"

namespace ift3100 {

enum ShaderType { NoShader, SobelFilter, GrayScale };

/**
 * @brief Class for imported models in order to map texture on them
 */
class Model {
	ofShader _sobelShader;
	ofShader _grayScaleShader;

	ofTexture	  _texture;
	of3dPrimitive _primitive;

public:
	ShaderType usingShader;

	Model(of3dPrimitive primitive);
	Model(ofMesh mesh, ofTexture texture = ofTexture());

	void draw();
	void loadTexture(std::string path);

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