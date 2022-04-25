#include "MaterialViewer.h"

namespace ift3100 {

MaterialViewer* MaterialViewer::_mv = nullptr;

MaterialViewer::MaterialViewer()
	: _target(ofSpherePrimitive())
	, illuminationStyle(IlluminationStyle::Default)
	, _primitiveType(PreviewPrimitiveType::Sphere) {
	_phong.load("../../src/Shaders/Lighting/Phong/phong.vert.glsl", "../../src/Shaders/Lighting/Phong/phong.frag.glsl");
	_lambert.load("../../src/Shaders/Lighting/Lambert/lambert.vert.glsl", "../../src/Shaders/Lighting/Lambert/lambert.frag.glsl");
	_gouraud.load("../../src/Shaders/Lighting/Gouraud/gouraud.vert.glsl", "../../src/Shaders/Lighting/Gouraud/gouraud.frag.glsl");
	_blinnphong.load("../../src/Shaders/Lighting/BlinnPhong/blinnphong.vert.glsl", "../../src/Shaders/Lighting/BlinnPhong/blinnphong.frag.glsl");

	_fbo.allocate(CAMERA_WIDTH, (CAMERA_WIDTH / _camera.getAspectRatio()));

	_camera.setPosition(0, 0, 100);
	_light.setPosition(200, 200, 200);
}

MaterialViewer* MaterialViewer::Get() {
	if (_mv == nullptr) {
		_mv = new MaterialViewer();
	}
	return _mv;
}

/**
 * @brief Draw the preview of target in the fbo. 
 * POV from the camera and light from the light. 
 * The illuminationStyle is provided by the IlluminationStyle enum 
 * reportoring all the shaders useable.
 */
void MaterialViewer::draw() {
	ofEnableDepthTest();
	ofEnableLighting();

	ofShader* illum = nullptr;
	if (illuminationStyle != IlluminationStyle::Default) {
		switch (illuminationStyle) {
			case Phong: illum = &_phong; break;
			case Lambert: illum = &_lambert; break;
			case Gouraud: illum = &_gouraud; break;
			case BlinnPhong: illum = &_blinnphong; break;
		}
		float array[] = {_light.getGlobalPosition().x, _light.getGlobalPosition().y, _light.getGlobalPosition().z};
		illum->begin();
		illum->setUniform3f("color_ambient", _light.getAmbientColor().r, _light.getAmbientColor().g, _light.getAmbientColor().b);
		illum->setUniform3f("color_diffuse", _light.getDiffuseColor().r, _light.getDiffuseColor().g, _light.getDiffuseColor().b);
		illum->setUniform3f("color_specular", _light.getDiffuseColor().r, _light.getDiffuseColor().g, _light.getDiffuseColor().b);
		illum->setUniform1f("brightness", _light.getDiffuseColor().getBrightness());
		illum->setUniform1i("nbLights", 1);
		illum->setUniform3fv("lightPos", array, 1);
	}

	_fbo.begin();
	{
		ofClear(120, 120, 120, 255);
		_light.enable();
		_camera.begin();
		_target.draw();
		_camera.end();
		_light.disable();
	}
	_fbo.end();

	if (illuminationStyle != IlluminationStyle::Default)
		illum->end();

	ofDisableLighting();
	ofDisableDepthTest();
}

/**
 * @brief Make a copy of the target allowing 
 * the mesh to be different and changeable without
 * influing on the 3D representation.
 */
void MaterialViewer::setTarget(Model& target) {
	// TODO: rajouter les matériaux quand ils seront là
	_target.setTexture(target.getTexture()->isAllocated() ? *target.getTexture() : ofTexture());

	// reload the mesh to bind correctly the texture
	// don't know why
	PreviewPrimitiveType prev = _primitiveType;
	setPrimitiveType(PreviewPrimitiveType::Sphere);
	setPrimitiveType(prev);
}

/**
 * @brief Change the mesh primitive
 * 
 * @param type 
 */
void MaterialViewer::setPrimitiveType(PreviewPrimitiveType type) {
	of3dPrimitive pt;
	_primitiveType = type;
	_camera.setPosition(0, 0, 100);
	switch (_primitiveType) {
		case PreviewPrimitiveType::Sphere: pt = ofSpherePrimitive(); break;

		case PreviewPrimitiveType::Cone: pt = ofConePrimitive(); break;

		case PreviewPrimitiveType::IcoSphere: pt = ofIcoSpherePrimitive(); break;

		case PreviewPrimitiveType::Cube:
			pt = ofBoxPrimitive();
			_camera.setPosition(0, 0, 200);
			break;
	}

	_target.setMesh(pt.getMesh());
}

} // namespace ift3100