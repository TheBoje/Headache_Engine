#include "Raytracing.h"
#include "Constant.h"

namespace ift3100 {

Raytracing::Raytracing(ofCamera* src, std::vector<ofLight*>& lights, std::vector<Model*>& objs)
	: _viewSource(src)
	, _lights(lights)
	, _objects(objs)
	, _result(ofImage()) { }

void Raytracing::render() {
	float	fov				= _viewSource->getFov();
	float	aspectRatio		= _viewSource->getAspectRatio();
	ofVec3f cameraDirection = _viewSource->getLookAtDir();
	ofVec3f cameraPosition	= _viewSource->getPosition();

	float w = CAMERA_WIDTH;
	float h = CAMERA_WIDTH * aspectRatio;

	_result.crop(0, 0, w, h);

	int resolution = 1;

	float		   nearest;
	of3dPrimitive* prim = nullptr;

	for (int x = 0; x < w; x += resolution) {
		for (int y = 0; y < h; y += resolution) {
			nearest	 = std::numeric_limits<float>::infinity();
			float dx = std::abs(x - w / 2);
			float dy = std::abs(y - h / 2);

			float Px = (2 * ((x + 0.5) / w) - 1) * std::tan(fov / 2 * M_PI / 180) * aspectRatio;
			float Py = (1 - 2 * ((y + 0.5) / h) * std::tan(fov / 2 * M_PI / 180));

			Ray ray(cameraPosition, ofVec3f(Px, Py, -1) - cameraPosition);

			// Check intersection for all surfaces.
			for (Model* obj : _objects) {
				Intersection inter = ray.intersect(obj->getPrimitive());

				if (inter.intersect && (inter.position - cameraPosition).length() < nearest) {
					nearest = (inter.position - cameraPosition).length();
					prim	= &obj->getPrimitive();
				}
			}

			// if there is no surfaces reached, we draw the default color
			if (prim == nullptr) {
				_result.setColor(x, y, ofColor(120, 120, 120));
			} else {
				for (ofLight* light : _lights) {
					// générer les rayons d'éclairage et d'ombrage
				}

				// gérer la reflexsivité et la transparence (refraction)

				// evaluer illumination avec nearest surface.
				// image[x, y] += équation du modèle d’illumination
			}
		}
	}
}

} // namespace ift3100