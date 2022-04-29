#ifndef MATERIA_VIEWER_H
#define MATERIA_VIEWER_H

#include "Model.h"
#include "ofFbo.h"
#include "ofLight.h"
#include "ofShader.h"
#include "ofCamera.h"
#include "Constant.h"

namespace ift3100 {

enum IlluminationStyle { Default, Phong, Lambert, Gouraud, BlinnPhong };
enum PreviewPrimitiveType { Sphere, Cube, IcoSphere, Cone };

/**
 * @brief Show a preview of the material depending of the light
 * and the illumination shader selected. 
 * 
 * Singleton is used for that class.
 */
class MaterialViewer {
	ofLight _light;

	ofShader _lambert;
	ofShader _phong;
	ofShader _gouraud;
	ofShader _blinnphong;

	PreviewPrimitiveType _primitiveType;
	Model				 _target;

	ofCamera _camera;
	ofFbo	 _fbo;

	MaterialViewer();

	static MaterialViewer* _mv;

public:
	static MaterialViewer* Get();

	IlluminationStyle illuminationStyle;

	void draw();

	void setTarget(Model& target);

	void setPrimitiveType(PreviewPrimitiveType type);

	inline ofFloatColor getLightColor() {
		return _light.getAmbientColor();
	}

	inline void setLightColor(ofColor color) {
		_light.setAmbientColor(color);
	}

	inline PreviewPrimitiveType getType() {
		return _primitiveType;
	}
	inline ofFbo& getFbo() {
		return _fbo;
	}
	inline ofMaterial& getMaterial() {
		return _target.getMaterial();
	}
	inline void setMaterial(const ofMaterial& mat) {
		_target.setMaterial(mat);
	}
};

} // namespace ift3100

#endif