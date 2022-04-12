#ifndef MODEL_H
#define MODEL_H

#include "of3dPrimitives.h"
#include "ofTexture.h"
#include "ofMesh.h"
#include "ofNode.h"
#include "Logger.h"

namespace ift3100 {

enum ShaderType { NoShader, SobelFilter };

/**
 * @brief Class for imported models in order to map texture on them
 */
class Model {
	ofShader _sobelShader;

	of3dPrimitive _primitive;
	ofTexture	  _texture;

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