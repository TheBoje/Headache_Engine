#include "Cubemap.h"
#include "Logger.h"

namespace ift3100 {
//--------------------------------------------------------------
Cubemap::Cubemap() { }

void Cubemap::setup(string spritePath, int size) {
	sprite.setup(ofImage(spritePath), size, size, 6);
	shader.load("../../src/Shaders/Skybox/skybox.vert.glsl", "../../src/Shaders/Skybox/skybox.frag.glsl");
	ofDisableArbTex();

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &textureObjectID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint pixelFormat = GL_RGB;
	if (sprite.getPart(0).getPixels().getNumChannels() == 4)
		pixelFormat = GL_RGBA;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, size, size, 0, pixelFormat, GL_UNSIGNED_BYTE, sprite.getPart(0).getPixels().getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, size, size, 0, pixelFormat, GL_UNSIGNED_BYTE, sprite.getPart(1).getPixels().getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, size, size, 0, pixelFormat, GL_UNSIGNED_BYTE, sprite.getPart(2).getPixels().getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, size, size, 0, pixelFormat, GL_UNSIGNED_BYTE, sprite.getPart(3).getPixels().getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, size, size, 0, pixelFormat, GL_UNSIGNED_BYTE, sprite.getPart(4).getPixels().getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, size, size, 0, pixelFormat, GL_UNSIGNED_BYTE, sprite.getPart(5).getPixels().getData());

	IFT_LOG << "done";
}

void Cubemap::bind(int pos) {
	bindID = pos;

	glActiveTexture(GL_TEXTURE0 + pos);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObjectID);
}

//--------------------------------------------------------------
void Cubemap::unbind() {
	glActiveTexture(GL_TEXTURE0 + bindID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
}

//--------------------------------------------------------------
void Cubemap::draw(ofVec3f pos) {
	shader.begin();

	bind();

	ofBoxPrimitive tmp(SIZE, SIZE, SIZE, 1, 1, 1);

	ofPushMatrix();
	ofTranslate(pos);
	tmp.getMesh().draw();
	ofPopMatrix();
	unbind();

	shader.end();
}
} // namespace ift3100