#include "Renderer.h"

namespace ift3100 {
	void Renderer::setup() {
		ofSetFrameRate(60);
		ofSetCircleResolution(32);
		backgroundColor = ofColor::darkGray;
        curseur.mouse_press_x = curseur.mouse_press_y = curseur.souris_courant_x = curseur.souris_courant_y = 0;
        curseur.is_mouse_button_pressed = false;
		ofLog() << "<renderer::setup> done";
	}

	void Renderer::update()
	{
		_time_current = ofGetElapsedTimef();
		_time_elapsed = _time_current - _time_last;
		_time_last = _time_current;
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
	void Renderer::addPrimitive(const ofVec4f& pos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor) {
		VectorPrimitive p;
		p.strokeWidth = strokeWidth;
		p.strokeColor = strokeColor;
		p.fillColor = fillColor;
		p.type = type;
		p.position1.x = pos.x;
		p.position1.y = pos.y;
		p.position2.x = pos.z;
		p.position2.y = pos.w;
		p.fill = fill;
		primitives.push_back(p);
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
		}
	}

	void Renderer::draw()
	{
		ofSetBackgroundColor(backgroundColor);
		// Draw primitives based on their data
		for (VectorPrimitive p : primitives) {
			if (p.fill) {
				ofFill();
			} else {
				ofNoFill();
			}
			ofSetLineWidth(p.strokeWidth);
            switch (p.type) {
				case Line:
					ofSetColor(p.strokeColor);
					ofDrawLine(p.position1, p.position2);
					break;
				case Rectangle:
					if (p.fill) {
						ofFill();
						ofSetColor(p.fillColor);
						ofDrawRectangle(p.position1, p.position2.x - p.position1.x, p.position2.y - p.position1.y);
					}
					ofNoFill();
					ofSetColor(p.strokeColor);
					ofDrawRectangle(p.position1, p.position2.x - p.position1.x, p.position2.y - p.position1.y);
					break;
				case Ellipse:
					if (p.fill) {
						ofFill();
						ofSetColor(p.fillColor);
						ofDrawEllipse(p.position1.x + (p.position2.x - p.position1.x) / 2.0f, p.position1.y + (p.position2.y - p.position1.y) / 2.0f, p.position2.x - p.position1.x, p.position2.y - p.position1.y);
					}
					ofNoFill();
					ofSetColor(p.strokeColor);
					ofDrawEllipse(p.position1.x + (p.position2.x - p.position1.x) / 2.0f, p.position1.y + (p.position2.y - p.position1.y) / 2.0f, p.position2.x - p.position1.x, p.position2.y - p.position1.y);
					break;
				default:
					ofLog() << "<renderer::draw> unknown type of primitive";
					break;
			}
		}
		// As ofFill / ofNoFill is modified with primitives
		ofFill();


        //afficher le curseur
        curseur.dessiner_curseur(curseur.souris_courant_x, curseur.souris_courant_y, curseur.interface.optionCursor);
    }
}