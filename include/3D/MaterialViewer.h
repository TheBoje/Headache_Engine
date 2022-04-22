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

	inline PreviewPrimitiveType getType() {
		return _primitiveType;
	}
	inline ofFbo& getFbo() {
		return _fbo;
	}
};

} // namespace ift3100

#endif