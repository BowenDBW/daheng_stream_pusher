#include "image_converter.h"

std::queue<unsigned char*>* ImageConverter::images = new std::queue<unsigned char*>();

int ImageConverter::totalReceive = 0;
int ImageConverter::fps = 0;
int ImageConverter::opencvWindowScale = 1;
int ImageConverter::opencvWindowBarWidth = 1;
int ImageConverter::handleMode = HandleMode::infoOnly;
int ImageConverter::imageWidth = DEFAULT_WIDTH;
int ImageConverter::imageHeight = DEFAULT_HEIGHT;

std::string ImageConverter::OPENCV_WINDOW_NAME = "opencv preview";

std::mutex* ImageConverter::mu = new std::mutex();
std::condition_variable* ImageConverter::cond = new std::condition_variable();

int ImageConverter::getWidth() {
	return imageWidth;
}

int ImageConverter::getHeight() {
	return imageHeight;
}

void ImageConverter::setFps(int newFps) {
	fps = newFps;
}

void ImageConverter::consumeImageThread() {
	while (true) {
		std::unique_lock<std::mutex> locker(*mu);
		while (images->empty())
			cond->wait(locker); // Unlock mu and wait to be notified
		unsigned char* data = images->front();

		switch (handleMode) {
			case HandleMode::opencvUi:
				showInOpencv(data);
				break;

			case HandleMode::localSave:
				localSave(data);
				break;

			case HandleMode::rtmp:
				rtmpStreaming(data);
				break;

			case HandleMode::infoOnly:
			default:
				infoOnly();
		};

		images->pop();
		locker.unlock();
	}
}

void ImageConverter::produceImageData(unsigned char* newBytes) {
	std::unique_lock<std::mutex> locker(*mu);
	images->push(newBytes);
	locker.unlock();
	cond->notify_one();
}

void ImageConverter::init(const int& width, const int& height) {
	imageWidth = width;
	imageHeight = height;
	opencvWindowScale = width / OPENCV_WINDOW_MAX_WIDTH;
	opencvWindowBarWidth = width % OPENCV_WINDOW_MAX_WIDTH;
	std::thread consumer(consumeImageThread);
	consumer.detach();
}

void ImageConverter::reset(const int& mode) {
	totalReceive = 0;
	handleMode = mode;
}

void ImageConverter::rtmpStreaming(unsigned char* data) {

}

void ImageConverter::localSave(unsigned char* data) {

}

void ImageConverter::showInOpencv(unsigned char* data) {

	cv::Mat image = arrayToCvMat(data);
	printInfos(image);

	cv::imshow(OPENCV_WINDOW_NAME, image);
	cv::waitKey(1);
}

void ImageConverter::infoOnly() {

	cv::Mat image = cv::Mat(720, 1280, CV_8UC3, cv::Scalar(255, 255, 255));

	printInfos(image);

	cv::imshow(OPENCV_WINDOW_NAME, image);
	cv::waitKey(1);
}

void ImageConverter::printInfos(cv::Mat image) {
	++totalReceive;

	std::string tooltip =  "frame: " + std::to_string(totalReceive)
		+ ", fps: " + std::to_string(fps)
		+ ", width: " + std::to_string(imageWidth)
		+ ", height: " + std::to_string(imageHeight);

	cv::putText(image, tooltip, cv::Point(10, 30),
		cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2, 2);
}

void ImageConverter::closeOpencvWindows() {
	cv::destroyAllWindows();
}

cv::Mat ImageConverter::arrayToCvMat(unsigned char* data) {
	cv::Mat image = cv::Mat(imageHeight, OPENCV_WINDOW_MAX_WIDTH,
		CV_8UC3, cv::Scalar(255, 255, 255));
	int matHeight = imageHeight / opencvWindowScale;
	//TODO: bug here
	for (int i = 0; i < OPENCV_WINDOW_MAX_WIDTH; i++) {
		for (int j = 0; j < matHeight; j++) {
			image.at<cv::Vec3b>(opencvWindowScale * i, i)[0]
				= static_cast<int>(data[opencvWindowScale * i * 3, opencvWindowScale * j]);
			image.at<cv::Vec3b>(opencvWindowScale * i, i)[1]
				= static_cast<int>(data[opencvWindowScale * i * 3 + 1, opencvWindowScale * j]);
			image.at<cv::Vec3b>(opencvWindowScale * i, i)[2]
				= static_cast<int>(data[opencvWindowScale * i * 3 + 2, opencvWindowScale * j]);
		}
	}
	return image;
}