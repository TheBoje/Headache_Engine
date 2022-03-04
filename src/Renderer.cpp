#include "Renderer.h"

namespace ift3100 {
	void Renderer::setup() {
		ofSetFrameRate(60);
		ofSetCircleResolution(32);
		backgroundColor = ofColor::darkGray;
		primitives.reserve(1000);
		ofLog() << "<renderer::setup> done";
	}

	void Renderer::update() {
		// Low framerate warning
		if (ofGetFrameRate() < 5 && ofGetFrameNum() > 5) {
			ofLog(OF_LOG_WARNING) << std::setprecision(2)
			<< "<renderer::update> frame:" << ofGetFrameNum() 
			<< " fps: " << ofGetFrameRate();
		}
	}

	/**
	 * Add a primitive in the render stack
	 * 
	 * @param mousePos position top-left & bottom-right corners of primitive (from mouse position)
	 * @param type type of primitive to draw. eg Line 
	 * @param strokeWidth
	 * @param strokeColor
	 * @param fill enable primitive filling
	 * @param fillColor 
	 * 
	*/
	void Renderer::addPrimitive(const ofVec4f& pos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor, int ttl) {
		primitives.push_back(VectorPrimitive(pos, type, strokeWidth, strokeColor, fill, fillColor, ttl));
		}

	/**
	 * Undo the last primitive added to the primitive stack (via Renderer::addPrimitive).
	*/
	void Renderer::undoPrimitive() {
		// Remove last primitive and give it to redoPrimitive, serving as history stack
		if (!primitives.empty()) {
			VectorPrimitive p = primitives.back(); 
			primitives.pop_back();
			redoPrimitives.push(p);
		} else {
			ofLog() << "<renderer::undoPrimitive> nothing to undo";
		}
	}

	/**
	 * Redo the last undo primitive (via Renderer::undoPrimitive).
	*/
	void Renderer::redoPrimitive() {
		// Push the newest primitive in stack history and remove it from stack
		if (!redoPrimitives.empty()) {
			primitives.push_back(redoPrimitives.top());
			redoPrimitives.pop();
		} else {
			ofLog() << "<renderer::redoPrimitive> nothing to redo";
		}
	}

	void Renderer::draw() {
		ofSetBackgroundColor(backgroundColor);
		// Draw primitives based on their data
		for (auto p = primitives.begin(); p != primitives.end();) {
			for (int i = 0; i < 2; i++) {
				if (i == 0 && p->FILL) {
					ofFill();
					ofSetColor(p->FILL_COLOR);
				} else { 
					ofNoFill();
					ofSetColor(p->STROKE_COLOR);
				}

				switch (p->getPrimitiveType()) {
					case Point: 
						ofDrawRectRounded(p->POSITION_2 - (p->STROKE_WIDTH / 2.0f), p->STROKE_WIDTH, p->STROKE_WIDTH, DEFAULT_RECTANGLE_ROUNDING);
						break;
					case Line:
						ofDrawLine(p->POSITION_1, p->POSITION_2);
						break;
					case Rectangle:
						ofDrawRectRounded(p->POSITION_1, p->POSITION_2.x - p->POSITION_1.x, p->POSITION_2.y - p->POSITION_1.y, DEFAULT_RECTANGLE_ROUNDING);
						break;
					case Ellipse:
						ofDrawEllipse(p->POSITION_1.x + (p->POSITION_2.x - p->POSITION_1.x) / 2.0f, p->POSITION_1.y + (p->POSITION_2.y - p->POSITION_1.y) / 2.0f, p->POSITION_2.x - p->POSITION_1.x, p->POSITION_2.y - p->POSITION_1.y);
						break;
					case Triangle:
						ofDrawTriangle(p->POSITION_1.x, p->POSITION_2.y, p->POSITION_1.x + (p->POSITION_2.x - p->POSITION_1.x) / 2.0f, p->POSITION_1.y, p->POSITION_2.x, p->POSITION_2.y);
						break;
					case Cross: {
						int sizeX = p->POSITION_1.x - p->POSITION_2.x;
						int sizeY = p->POSITION_1.y - p->POSITION_2.y;
						ofDrawRectRounded(p->POSITION_2.x, p->POSITION_2.y + 2.0f*(sizeY / 5.0f), sizeX, sizeY / 5.0f, DEFAULT_RECTANGLE_ROUNDING);
						ofDrawRectRounded(p->POSITION_2.x + 2.0f*(sizeX / 5.0f), p->POSITION_2.y, sizeX / 5.0f, sizeY, DEFAULT_RECTANGLE_ROUNDING);
						break;
					}
					case Star: {
						int sizeX = p->POSITION_1.x - p->POSITION_2.x;
						int sizeY = p->POSITION_1.y - p->POSITION_2.y;
						ofDrawTriangle(p->POSITION_2.x, p->POSITION_2.y + (sizeY / 4.0f), p->POSITION_1.x, p->POSITION_2.y + (sizeY / 4.0f), p->POSITION_2.x + (sizeX / 2.0f), p->POSITION_1.y);
						ofDrawTriangle(p->POSITION_2.x, p->POSITION_1.y - (sizeY / 4.0f), p->POSITION_1.x, p->POSITION_1.y - (sizeY / 4.0f), p->POSITION_2.x + (sizeX / 2.0f), p->POSITION_2.y);
						break;
					}
				}
			}
			// Update time to live
			p->TTL-=1;
			// Update vector iterator
			p++;
		}

		// Remove ttl = 0 (dead) primitives
		primitives.erase(std::remove_if(primitives.begin(), primitives.end(),
		[](const VectorPrimitive& p) { 
			return p.TTL == 0; // put your condition here
		}), primitives.end());


		// As ofFill / ofNoFill is modified with primitives
		ofFill();

        //afficher le curseur
        // curseur.dessiner_curseur(curseur.souris_courant_x, curseur.souris_courant_y);
    }
}