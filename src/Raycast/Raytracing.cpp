#include "Raytracing.h"
#include "Constant.h"
#include "Renderer3D.h"
#include <cstdlib>

namespace ift3100 {

Raytracing::Raytracing(ofCamera* src, std::vector<ofLight*>& lights, std::vector<Model*>& objs)
	: _viewSource(src)
	, _lights(lights)
	, _objects(objs)
	, _result(ofImage()) { }

void Raytracing::getIntersectionInWorld(Ray& ray, int* indexHitobj, int* indexHitLight, Intersection* inter) {
	float nearest = std::numeric_limits<float>::infinity();
	ofVec3f rayPosition = ray.getOrigin();
	*indexHitLight = -1;
	*indexHitobj = -1;

	// Get the nearest object hit by the ray
	for (int i = 0; i < _objects.size(); i++) {
		Intersection tmpinter = ray.intersect(_objects[i]->getPrimitive());
		if (tmpinter.intersect && (tmpinter.position - rayPosition).length() < nearest) {
			nearest = (tmpinter.position - rayPosition).length();
			*indexHitobj = i;
			*inter = tmpinter;
		}
	}

	for (int i = 0; i < _lights.size(); i++) {
		Intersection tmpinter;
		tmpinter = ray.intersect(*_lights[i]);

		if (tmpinter.intersect && (tmpinter.position - rayPosition).length() < nearest) {
			nearest = (tmpinter.position - rayPosition).length();
			*indexHitLight = i;
			*indexHitobj = -1;
			*inter = tmpinter;
		}
	}
}

/**
 * @brief Get a Random Vector In Hemisphere defined by the normalized vector "normal"
 *
 * @param normal
 * @return ofVec3f
 */
ofVec3f getRandomVectorInHemisphere(const ofVec3f& normal) {
	float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	ofVec3f direction = ofVec3f(x, y, z);
	float xrot = std::acos(normal.dot(ofVec3f(1, 0, 0)));
	float yrot = std::acos(normal.dot(ofVec3f(0, 1, 0)));
	float zrot = std::acos(normal.dot(ofVec3f(0, 0, 1)));

	return ofVec3f(x, y, z).rotate(xrot, yrot, zrot);
}

ofVec3f Raytracing::lightspath(const Intersection& inter, int from) {
	ofVec3f resultColor(0, 0, 0);

	bool isAmbiantSet = false, isEmmisiveSet = false;

	// choper toutes les sources de lumières
	for (ofLight* light : _lights) {
		// vérifier si l.n < 0 l : pos -> lumière n : normale à la surface
		if (((ofVec3f)(inter.position - light->getPosition())).dot(inter.normal) > 0)
			continue;

		int hitobj = -1;
		int hitlight = -1;
		Intersection interLight;
		Ray ray(inter.position, light->getPosition() - inter.position);
		getIntersectionInWorld(ray, &hitobj, &hitlight, &interLight);

		// vérifier si il y a un mesh qui bloque la source d'émission
		if (hitobj == from) {
			float dist = (inter.position - light->getPosition()).length();
			float attenuation =
				(light->getAttenuationConstant() + light->getAttenuationLinear() * dist + light->getAttenuationQuadratic() * dist * dist);
			float ratio = 1 / (attenuation == 0 ? 0.001 : attenuation);
			// TODO: diffuse color est à chier ?
			ofColor diffuse = _objects[from]->getMaterial().getDiffuseColor();

			ofVec3f L = ((ofVec3f)(light->getPosition() - inter.position)).getNormalized();
			ofVec3f V = ((ofVec3f)(_viewSource->getPosition() - inter.position)).getNormalized();

			float diffuseReflexion = std::max(inter.normal.dot(L), 0.0f);
			float specReflexion = 0;
			if (diffuseReflexion > 0) {
				specReflexion =
					std::pow(std::max(inter.normal.dot((L + V) / (L + V).length()), 0.0f), _objects[from]->getMaterial().getShininess() / 4);
			}

			ofColor ca = light->getAmbientColor() * _objects[from]->getMaterial().getAmbientColor();
			ofColor cd = light->getDiffuseColor() * _objects[from]->getMaterial().getDiffuseColor();
			ofColor cs = light->getSpecularColor() * _objects[from]->getMaterial().getSpecularColor();
			ofColor ce = _objects[from]->getMaterial().getEmissiveColor();

			resultColor +=
				ofVec3f(cd.r, cd.g, cd.b) * diffuseReflexion + ofVec3f(cs.r, cs.g, cs.b) * specReflexion +
				(isAmbiantSet ? ofVec3f(0) : ofVec3f(ca.r, ca.g, ca.b)) +
				(isEmmisiveSet ?
						  ofVec3f(0) :
						  ofVec3f(ce.r, ce.g, ce.b)); // * (light->getDiffuseColor().r, light->getDiffuseColor().g, light->getDiffuseColor().b) * ratio;
			isAmbiantSet = true;
			isEmmisiveSet = true;
		}
	}

	return resultColor;
}

/**
 * @brief Path tracing algorithm from https://en.wikipedia.org/wiki/Path_tracing#Algorithm
 *
 * @param ray
 * @param depth
 * @return ofColor
 */
ofVec3f Raytracing::tracepath(Ray& ray, int depth) {
	if (depth >= MAX_DEPTH) {
		return ofVec3f(0, 0, 0); // Bounced enough times.
	}

	ofVec3f cameraPosition = _viewSource->getPosition();

	ofVec3f surfaceColor(0, 0, 0);

	int hitobj = -1;
	int li = -1;
	Intersection inter;
	inter.intersect = false;
	getIntersectionInWorld(ray, &hitobj, &li, &inter);

	float bias = 1e-4;

	if (hitobj != -1) {
		ofColor diffuse = _objects[hitobj]->getMaterial().getDiffuseColor();
		if ((_objects[hitobj]->transparency > 0 || _objects[hitobj]->reflection > 0) && depth < MAX_DEPTH) {
			float facingratio = ofClamp(-ray.getDirection().dot(inter.normal), -1, 1);
			float fresneleffect = ofLerp(std::pow(1 - facingratio, 3), 1, 0.1);
			ofVec3f bias = 0.001 * inter.normal;

			Ray reflectray = ray.reflect(inter);
			reflectray.setOrigin(reflectray.getOrigin() + bias);

			ofVec3f reflectionColor = tracepath(reflectray, depth + 1);
			ofVec3f refraction(0, 0, 0);

			// if(_objects[hitobj]->transparency > 0) {
			// 	Ray refrdir = ray.refract(inter, 1, 1);
			// 	refrdir.setOrigin(refrdir.getOrigin() - bias);
			// 	refraction = tracepath(refrdir, depth + 1);
			// }
			surfaceColor = (reflectionColor * fresneleffect + refraction * (1 - fresneleffect));
		}
		surfaceColor += lightspath(inter, hitobj);
	}

	return surfaceColor;
}

void Raytracing::render(int pxRes) {
	IFT_LOG << "starting";
	float fov = _viewSource->getFov();
	float aspectRatio = _viewSource->getAspectRatio();
	ofVec3f cameraDirection = _viewSource->getLookAtDir();
	ofVec3f cameraPosition = _viewSource->getPosition();

	float w = CAMERA_WIDTH / pxRes;
	float h = (int)((CAMERA_WIDTH / aspectRatio) / pxRes);

	_result.allocate(w, h, ofImageType::OF_IMAGE_COLOR);

	int resolution = 1;

	Model* hitobj = nullptr;

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			float Px = (2 * ((x + 0.5) / w) - 1) * std::tan(fov / 2 * M_PI / 180) * aspectRatio;
			float Py = (1 - 2 * ((y + 0.5) / h) * std::tan(fov / 2 * M_PI / 180));

			Ray ray(cameraPosition, ofVec3f(Px, Py, -1) - cameraPosition);

			// vec3 to unbound 255
			ofVec3f pixelColor(0, 0, 0);

			for (int i = 0; i < resolution; i++) {
				pixelColor += tracepath(ray, 0);
			}
			pixelColor /= resolution; // Average samples.
			pixelColor = pixelColor.getNormalized();

			// IFT_LOG << "(" << x << " " << y << ")"
			// << "color " << ofColor(pixelColor.x * 255, pixelColor.y * 255, pixelColor.z * 255);

			_result.getPixelsRef().setColor(x, y, ofColor(pixelColor.x * 255, pixelColor.y * 255, pixelColor.z * 255));
		}
	}
	IFT_LOG << "done";
}

void Raytracing::saveImage() {
	_result.save("render.png");
}

} // namespace ift3100