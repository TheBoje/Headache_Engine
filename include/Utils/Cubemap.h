#ifndef IFT_3100_CUBEMAP_H
#define IFT_3100_CUBEMAP_H

#include "Sprite.h"
#include "ofMain.h"

namespace ift3100 {
class Cubemap {
	static const int SIZE = 2000;

public:
	Cubemap();

	/**
	 * @param spritePath path to sprite image
	 * @param size size in px of a single image in the sprite
	 *
	 * Note: the sprite must contain 6 images, using the following pattern.
	 * For traditionnal Cube maps images, the pattern is
	 * |   | 1 |   |   |
	 * | 2 | 3 | 4 | 5 |
	 * |   | 6 |   |   |
	 * Here, we expect (following above pattern) :
	 * | 4 | 2 | 1 | 6 | 3 | 5 |
	 *
	 * Note2: yes this pattern is quite wierd
	 * TODO: Maybe change the expected pattern to:
	 * | 2 | 3 | 4 | 5 | 1 | 6 | or something along those lignes
	*/
	void setup(string spritePath, int size);
	void draw(ofVec3f pos);

	void bind(int pos = 0);
	void unbind();

private:
	Sprite	 sprite;
	ofShader shader;
	uint	 textureObjectID;
	int		 bindID;
};
} // namespace ift3100

#endif