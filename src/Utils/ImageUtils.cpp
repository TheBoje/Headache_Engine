#include "ImageUtils.h"

namespace ift3100 {
ofImage* ImageUtils::importImage(const std::string& path) {
	IFT_LOG << "from " << path;
	return new ofImage(path);
}

void ImageUtils::exportImage(const std::string& name) {
	IFT_LOG << "exporting [" << name << "] - (" << ofGetWindowWidth() << ", " << ofGetWindowHeight() << ")";
	ofImage res;
	res.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	res.save(name);
}

/**
     * Compute the histogram of the three colors of the image.
     * Allocate a 2 dimensional array and give the use the property, it's up to you
     * to delete it.
     *
     * @param image
     * @param space
     * @return a 3 lengthen array of 256 lengthen unsigned int vectors for rbg colors
     */
unsigned int** ImageUtils::computeHistRGB(const ofImage& image) {
	int			   channels = 3;
	unsigned int** hist		= new unsigned int*[channels];

	for (int i = 0; i < channels; i++)
		hist[i] = new unsigned int[256]();

	for (int i = 0; i < (int)image.getWidth(); i++) {
		for (int j = 0; j < (int)image.getHeight(); j++) {
			ofColor color = image.getColor(i, j);
			hist[0][color.r]++;
			hist[1][color.g]++;
			hist[2][color.b]++;
		}
	}

	return hist;
}

/**
     * Convert an image using openCV library. Result image can be manipulated using openCV lib. For
     * example you can use it this way :
     *
    // cvt to hsv
    cv::Mat res = ImageUtils::convert(input, cv::COLOR_RGB2HSV);
    cv::Mat channels[3];
    cv::split(res, channels); // split image channels
    channels[1] = 1; // Set saturation to 1 (max)
    cv::merge(channels, 3, res); // Merge channels back
    cv::cvtColor(res, res, cv::COLOR_HSV2RGB); // Convert back to RBG, ofImage format
    ofImage* output = new ofImage();
    ofxCv::toOf(res, *output); // convert to ofImage
     *
     * @param image
     * @param code convertion code, see https://docs.opencv.org/3.1.0/de/d25/imgproc_color_conversions.html
     * @return cv image converted by `code`
     */
cv::Mat ImageUtils::convert(const ofImage& image, int code) {
	cv::Mat tmp = ofxCv::toCv(image); // Convert from ofImage to cv::Mat (openCV image object)
	cv::Mat converted;
	cv::cvtColor(tmp, converted, code); // Convert color
	return converted;
}
} // namespace ift3100