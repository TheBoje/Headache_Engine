#include "Application.h"
#include "ImageUtils.h"
#include "Hierarchy.h"

namespace ift3100 {
	// fonction d'initialisation de l'application
	void Application::setup()
	{
		ofSetWindowTitle("IFT-3100 Main");

		renderer.setup();
        interface.construire_interface();

		ofLog() << "<app::setup>";
	}

	// fonction de mise à jour de la logique de l'application
	void Application::update()
	{
        // assigner les états courants de l'interface
        //ofBackground(interface.color_picker_background);
        renderer.stroke_color = interface.color_picker_stroke;
        renderer.stroke_weight = interface.slider_stroke_weight;
        renderer.text = interface.textbox;
        renderer.update();
	}

	// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
	void Application::draw()
	{
        renderer.draw();
        ofBackground(interface.color_picker_background);
        if (interface.checkbox) {
            ofSetColor(interface.rInterface, interface.gInterface, interface.bInterface, interface.aInterface);
            ofDrawCircle(interface.xInterface, interface.yInterface, interface.rayonInterface);
            interface.gui.draw();
        }
	}

	// fonction appelée juste avant de quitter l'application
	void Application::exit()
	{
        //interface.button.removeListener(this, &Application::button_pressed);
        ofLog() << "<app::exit>";
	}

	void Application::addImage(ofImage * image) {
		ofLog() << "<app:addImage> x:" << image->getHeight() << " y:" << image->getWidth();
		renderer._image = image;
	}

	void Application::keyReleased(int key)
	{
  		ofLog() << "<app::keyReleased: " << key << ">";

		if (key == ' ') {
			ift3100::ImageUtils::exportImage("render.png");
		}
        if (key == 117) // touche u
        {
            interface.checkbox = !interface.checkbox;
            ofLog() << "<toggle ui: " << interface.checkbox << ">";
        }
	}

	void Application::dragEvent(ofDragInfo dragInfo) {
		ofLog() << "<app::ofDragInfo> " << dragInfo.files.at(0);

		if (renderer._image == nullptr) {
			renderer._image = new ofImage();
		}

		renderer._image->load(dragInfo.files.at(0));

		if (renderer._image->getWidth() > 0 && renderer._image->getHeight() > 0)
			ofSetWindowShape(renderer._image->getWidth(), renderer._image->getHeight());
		}

    void Application::windowResized(int w, int h)
    {
        ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
    }
}
