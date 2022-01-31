#include "Sprites.h"

/**
 * Create a sprite using an ofImage. Will create part_counts patch of the image of same part_height and part_width.
 * @param img
 * @param part_height
 * @param part_width
 * @param part_counts
 */
ift3100::Sprites::Sprites(const ofImage &img, int part_height, int part_width, size_t part_counts) :
    _parts_count(part_counts),
    _parts_height(part_height),
    _parts_width(part_width),
    _index(0)
{
    const ofPixels& pixels = img.getPixels();

    size_t height = pixels.getHeight();
    size_t width = pixels.getWidth();

    unsigned int nb_rows = height / part_height;
    unsigned int nb_cols = width / part_width;

    _image_parts.reserve(_parts_count);

    for(int part_index = 0; part_index < _parts_count; ++part_index) {
        ofPixels part;
        part.allocate(_parts_width, _parts_height, img.getImageType());

        // x and y refers to patch origin position in the original image
        size_t x = (part_index % nb_cols) * part_width;
        size_t y = (part_index / nb_cols) * part_height;

        // copy the part_index'th pixels in a new patch "part"
        for(size_t i = x; i < (x + part_width); ++i) {
            for(size_t j = y; j < (y + part_height); ++j) {
                part[(i - x) * nb_rows + (j - y)] = pixels[i * nb_rows + j];
            }
        }

        _image_parts.emplace_back(ofImage(part));
    }
}

ift3100::Sprites::~Sprites() = default;

ofImage ift3100::Sprites::next() {
    _index = (_index + 1) % _parts_count;
    return _image_parts.at(_index);
}

ofImage ift3100::Sprites::getPart(size_t index) {
    return _image_parts.at(index);
}

/**
 * Concatenate two sprites together, each patch in the sprite passed as arg is placed after the sprite object
 * @param sprite
 */
void ift3100::Sprites::concatenate(const ift3100::Sprites &sprite) {
    if(_parts_height == sprite._parts_height && _parts_width == sprite._parts_width) {
        _parts_count = _parts_count + sprite._parts_count;
        _image_parts.insert(_image_parts.end(), sprite._image_parts.begin(), sprite._image_parts.end());
    }
}

/**
 * Simply create a sprite of the image with the same patch height and width and concatenate it
 * @param image
 * @param part_counts
 */
void ift3100::Sprites::concatenate(const ofImage &image, size_t part_counts) {
    Sprites s(image, _parts_height, _parts_width, part_counts);
    concatenate(s);
}


