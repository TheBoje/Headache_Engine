#ifndef IFT_3100_SPRITES_H
#define IFT_3100_SPRITES_H

#include <iostream>
#include <vector>

#include "ofImage.h"


namespace ift3100 {
    class Sprites {
        std::vector<ofImage> _image_parts;  // Contains all the parts of the initial image (all the sprites)
        size_t _parts_count;                // The amount of parts (aka: _image_parts.size())

        int _parts_height;                  // The height of a part (each part need to have the same heigt and width
        int _parts_width;

        size_t _index;                      // The actual index of the iterator in _image_parts

    public:
        Sprites(const ofImage & img, int part_height, int part_width, size_t part_counts);
        ~Sprites();

        void concatenate(const Sprites & sprite);
        void concatenate(const ofImage & image, size_t part_counts);

        ofImage next();
        ofImage getPart(size_t index);
    };
}




#endif //IFT_3100_SPRITES_H
