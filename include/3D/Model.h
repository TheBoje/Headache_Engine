#ifndef MODEL_H
#define MODEL_H

#include "of3dPrimitives.h"
#include "ofTexture.h"
#include "ofMesh.h"
#include "ofNode.h"

namespace ift3100 {

/**
 * @brief Class for imported models in order to map texture on them
 */
class Model {
	of3dPrimitive _primitive;
	ofTexture	  _texture;

public:
	Model(of3dPrimitive primitive);
	Model(ofMesh mesh, ofTexture texture = ofTexture());

	void draw();

	inline ofMesh* getMesh() {
		return _primitive.getMeshPtr();
	}

	inline ofNode* getNode() {
		return &_primitive;
	}

	inline void setTexture(ofTexture texture) {
		_texture = texture;
	}
};
} // namespace ift3100

#endif