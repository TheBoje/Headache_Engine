#include "Renderer.h"

namespace ift3100 {
	void Renderer::setup()
	{
		ofSetFrameRate(60);
	}

	void Renderer::update()
	{
		_time_current = ofGetElapsedTimef();
		_time_elapsed = _time_current - _time_last;
		_time_last = _time_current;

		if (ofGetFrameRate() < 5) {
			ofLog() << std::setprecision(2)
			<< "<frame:" << ofGetFrameNum() << ">"
			<< " fps: " << ofGetFrameRate() ;
		}
	}

	void Renderer::draw()
	{

        if (_image != nullptr) {
			ofSetWindowShape(  _image->getWidth(), _image->getHeight());
			_image->draw(0, 0, _image->getWidth(), _image->getHeight());
		} else {
			ofColor c;
			c.setHsb(ofGetFrameNum(), 255, 255); // Cool gradiant using HSL 
			ofClear(c);
		}

        //afficher le curseur
        curseur.dessiner_curseur(curseur.souris_courant_x, curseur.souris_courant_y);
    }


}