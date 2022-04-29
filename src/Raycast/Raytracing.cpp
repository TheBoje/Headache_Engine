#include "Raytracing.h"
#include "Constant.h"
#include <cstdlib>
#include "Renderer3D.h"

namespace ift3100 {

Raytracing::Raytracing(ofCamera* src, std::vector<ofLight*>& lights, std::vector<Model*>& objs)
	: _viewSource(src)
	, _lights(lights)
	, _objects(objs)
	, _result(ofImage()) { }

void Raytracing::getIntersectionInWorld(Ray& ray, int* indexHitobj, int* indexHitLight, Intersection* inter) {
	float	nearest		= std::numeric_limits<float>::infinity();
	ofVec3f rayPosition = ray.getOrigin();
	*indexHitLight		= -1;
	*indexHitobj		= -1;

	// Get the nearest object hit by the ray
	for (int i = 0; i < _objects.size(); i++) {
		Intersection tmpinter = ray.intersect(_objects[i]->getPrimitive());
		if (tmpinter.intersect && (tmpinter.position - rayPosition).length() < nearest) {
			nearest		 = (tmpinter.position - rayPosition).length();
			*indexHitobj = i;
			*inter		 = tmpinter;
		}
	}

	for (int i = 0; i < _lights.size(); i++) {
		Intersection tmpinter;
		tmpinter = ray.intersect(*_lights[i]);

		if (tmpinter.intersect && (tmpinter.position - rayPosition).length() < nearest) {
			nearest		   = (tmpinter.position - rayPosition).length();
			*indexHitLight = i;
			*indexHitobj   = -1;
			*inter		   = tmpinter;
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
	float	xrot	  = std::acos(normal.dot(ofVec3f(1, 0, 0)));
	float	yrot	  = std::acos(normal.dot(ofVec3f(0, 1, 0)));
	float	zrot	  = std::acos(normal.dot(ofVec3f(0, 0, 1)));

	return ofVec3f(x, y, z).rotate(xrot, yrot, zrot);
}

ofColor Raytracing::lightspath(const Intersection& inter, int from) {
	ofColor resultColor = ofColor().black;

	// choper toutes les sources de lumières
	for (ofLight* light : _lights) {
		// vérifier si l.n < 0 l : pos -> lumière n : normale à la surface
		if (((ofVec3f)(inter.position - light->getPosition())).dot(inter.normal) > 0)
			continue;

		int			 hitobj	  = -1;
		int			 hitlight = -1;
		Intersection interLight;
		Ray			 ray(inter.position, light->getPosition() - inter.position);
		getIntersectionInWorld(ray, &hitobj, &hitlight, &interLight);

		// vérifier si il y a un mesh qui bloque la source d'émission
		if (hitobj == from) {
			float dist = (inter.position - light->getPosition()).length();
			float attenuation =
				(light->getAttenuationConstant() + light->getAttenuationLinear() * dist + light->getAttenuationQuadratic() * dist * dist);
			float ratio = 1 / (attenuation == 0 ? 1 : attenuation);
			// TODO: here only for point light, compute for all light types
			resultColor += light->getDiffuseColor() * ratio;
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
ofColor Raytracing::tracepath(Ray& ray, int depth) {
	if (depth >= MAX_DEPTH) {
		return ofColor().black; // Bounced enough times.
	}

	ofVec3f cameraPosition = _viewSource->getPosition();

	int			 hitobj = -1;
	int			 li		= -1;
	Intersection inter;
	inter.intersect = false;
	getIntersectionInWorld(ray, &hitobj, &li, &inter);

	if (inter.intersect) {
		// récupérer le matériau de l'objet hit

		// Tirer un rayon aléatoire dans l'hémisphère de la normale
		ofVec3f nextRayDirection = getRandomVectorInHemisphere(inter.normal);
		Ray		nextRay(inter.position, nextRayDirection);

		const float p		  = 1 / (2 * M_PI);
		float		cos_theta = nextRayDirection.dot(inter.normal);
		ofColor		BRDF	  = 1 / M_PI; // material.reflectance / PI;
		// ofColor incoming = tracepath(nextRay, depth + 1);

		if (hitobj != -1) {
			// BRDF = hitobj->getMaterial().t
			ofColor lightcolor = lightspath(inter, hitobj);
			return _objects[hitobj]->getMaterial().getEmissiveColor() + lightcolor + (BRDF * cos_theta / p);
		}

		return (BRDF * cos_theta / p); // + material.emittance
	} else {
		return ofColor().black;
	}
}

void Raytracing::render() {
	float	fov				= _viewSource->getFov();
	float	aspectRatio		= _viewSource->getAspectRatio();
	ofVec3f cameraDirection = _viewSource->getLookAtDir();
	ofVec3f cameraPosition	= _viewSource->getPosition();

	float w = 100; //CAMERA_WIDTH;
	float h = 100; //CAMERA_WIDTH * aspectRatio;

	_result.allocate(w, h, ofImageType::OF_IMAGE_COLOR);

	int resolution = 1;

	float  nearest;
	Model* hitobj = nullptr;

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			nearest = std::numeric_limits<float>::infinity();

			float Px = (2 * ((x + 0.5) / w) - 1) * std::tan(fov / 2 * M_PI / 180); // * aspectRatio;
			float Py = (1 - 2 * ((y + 0.5) / h) * std::tan(fov / 2 * M_PI / 180));

			Ray ray(cameraPosition, ofVec3f(Px, Py, -1) - cameraPosition);

			// vec3 to unbound 255
			ofVec3f pixelColor(0, 0, 0);

			for (int i = 0; i < resolution; i++) {
				ofColor c = tracepath(ray, 0);
				pixelColor += ofVec3f(c.r, c.g, c.b);
			}
			pixelColor /= resolution; // Average samples.
			IFT_LOG << "(" << x << " " << y << ")"
					<< "color " << pixelColor;
			_result.getPixelsRef().setColor(x, y, ofColor(pixelColor.x, pixelColor.y, pixelColor.z));
		}
	}
}

void Raytracing::saveImage() {
	_result.save("render.png");
}

} // namespace ift3100