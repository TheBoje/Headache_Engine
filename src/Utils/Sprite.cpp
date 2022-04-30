#include "Sprite.h"

namespace ift3100 {

Sprite::Sprite() { }

Sprite::~Sprite() = default;

/**
 * Create a sprite using an ofImage. Will create part_counts patch of the image of same part_height and part_width.
 * @param img the image that will be cut
 * @param part_height the height of one patch
 * @param part_width the width of one patch
 * @param part_counts the number of patches
 */
void Sprite::setup(const ofImage& img, int part_height, int part_width, std::size_t part_counts) {
	_parts_count = part_counts;
	_parts_width = part_width;
	_parts_height = part_height;
	_index = 0;
	const ofPixels& pixels = img.getPixels();

	std::size_t width = pixels.getWidth();

	unsigned int nb_cols = width / part_width;

	_image_parts.reserve(_parts_count);

	for (unsigned int part_index = 0; part_index < _parts_count; ++part_index) {
		ofImage part;
		part.allocate(_parts_width, _parts_height, img.getImageType());

		// x and y refers to patch origin position in the original image
		std::size_t x = (part_index % nb_cols) * part_width;
		std::size_t y = (part_index / nb_cols) * part_height;

		// copy the part_index'th pixels in a new patch "part"
		for (std::size_t i = x; i < (x + part_width); ++i) {
			for (std::size_t j = y; j < (y + part_height); ++j) {
				part.setColor(i - x, j - y, img.getColor(i, j));
			}
		}
		_image_parts.emplace_back(part);
	}
}

/**
 * @return the next image in the sprite order
 */
ofImage Sprite::next() {
	_index = (_index + 1) % _parts_count;
	return _image_parts.at(_index);
}

ofImage ift3100::Sprite::getPart(std::size_t index) {
	return _image_parts.at(index);
}

/**
 * Concatenate two sprites together, each patch in the sprite passed as arg is placed after the sprite object
 * @param sprite
 */
void Sprite::concatenate(const ift3100::Sprite& sprite) {
	if (_parts_height == sprite._parts_height && _parts_width == sprite._parts_width) {
		_parts_count = _parts_count + sprite._parts_count;
		_image_parts.insert(_image_parts.end(), sprite._image_parts.begin(), sprite._image_parts.end());
	}
}

/**
 * Simply create a sprite of the image with the same patch height and width and concatenate it
 * @param image
 * @param part_counts
 */
void Sprite::concatenate(const ofImage& image, std::size_t part_counts) {
	Sprite s;
	s.setup(image, _parts_height, _parts_width, part_counts);
	concatenate(s);
}
} // namespace ift3100