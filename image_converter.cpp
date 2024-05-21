#include "image_converter.h"

bool ImageConverter::showHud = false;
bool ImageConverter::saveImage = false;
bool ImageConverter::pushStream = false;
bool ImageConverter::showExternOpencvWindow = false;

int ImageConverter::totalReceive = 0;
int ImageConverter::fps = 0;
int ImageConverter::imageWidth = DEFAULT_WIDTH;
int ImageConverter::imageHeight = DEFAULT_HEIGHT;
int ImageConverter::saveInterval = 100;

std::string ImageConverter::rtmpUrl = "http://127.0.0.1/4000";
std::string ImageConverter::OPENCV_WINDOW_NAME = "opencv preview";
std::string ImageConverter::fileSavePath = "./";

std::mutex* ImageConverter::mu = new std::mutex();
std::condition_variable* ImageConverter::cond = new std::condition_variable();
std::queue<unsigned char*>* ImageConverter::images = new std::queue<unsigned char*>();

QLabel* ImageConverter::pixmap = nullptr;

int ImageConverter::getWidth() {
	return imageWidth;
}

int ImageConverter::getHeight() {
	return imageHeight;
}

int ImageConverter::getSaveInterval() {
	return saveInterval;
}

std::string ImageConverter::getRtmpUrl() {
	return rtmpUrl;
}

std::string ImageConverter::getFileSavePath() {
	return fileSavePath;
}

void ImageConverter::setFps(int newFps) {
	fps = newFps;
}

void ImageConverter::setFileSavePath(std::string path) {
	fileSavePath = path;
}

void ImageConverter::setSaveInterval(int interval) {
	saveInterval = interval;
}

void ImageConverter::setRtmpUrl(std::string url) {
	rtmpUrl = url;
}

void ImageConverter::setPixmap(QLabel* label) {
	pixmap = label;
}

void ImageConverter::setOption(bool shallShowHud, bool shallSaveImage, 
	bool shallPushStream, bool shallShowExternOpencvWindow) {

	showHud = shallShowHud;
	saveImage = shallSaveImage;
	pushStream = shallPushStream;
	showExternOpencvWindow = shallShowExternOpencvWindow;
}

void ImageConverter::consumeImageThread() {
	while (true) {
		std::unique_lock<std::mutex> locker(*mu);
		while (images->empty())
			cond->wait(locker); // Unlock mu and wait to be notified
		unsigned char* data = images->front();

		++totalReceive;

		cv::Mat* rawMat = arrayToCvMat(data);

		std::thread thread2(showInPixmap, rawMat);
		std::thread thread3(localSave, rawMat);
		std::thread thread4(rtmpStreaming, rawMat);

		showInOpencv(rawMat);

		thread2.join();
		thread3.join();
		thread4.join();
		
		delete rawMat;
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
	std::thread consumer(consumeImageThread);
	consumer.detach();
}

void ImageConverter::reset() {
	totalReceive = 0;
}

void ImageConverter::rtmpStreaming(cv::Mat* rawMat) {

	if (pushStream) {

	}
}

void ImageConverter::localSave(cv::Mat* rawMat) {

	if (saveImage) {
		if (totalReceive % saveInterval == 0) {
			std::string title = 
				fileSavePath + "/" + std::to_string(totalReceive) + ".jpg";
			cv::imwrite(title, *rawMat);
		}
	}
}

void ImageConverter::showInOpencv(cv::Mat* rawMat) {

	if (showExternOpencvWindow) {
		cv::Mat image = rawToDisplayMat(rawMat);
		
		cv::imshow(OPENCV_WINDOW_NAME, image);
		cv::waitKey(20);
	}
}

void ImageConverter::showInPixmap(cv::Mat* rawMat) {
	cv::Mat image = rawToDisplayMat(rawMat);
	QImage qImg = QImage((const unsigned char*)(image.data), image.cols, 
		image.rows, image.step, QImage::Format_RGB888);
	pixmap->setPixmap(QPixmap::fromImage(qImg));
}

void ImageConverter::printInfos(cv::Mat image, bool darkText) {
	std::string tooltip = "frame: " + std::to_string(totalReceive)
		+ ", fps: " + std::to_string(fps)
		+ ", width: " + std::to_string(imageWidth)
		+ ", height: " + std::to_string(imageHeight);

	if (darkText) {
		cv::putText(image, tooltip, cv::Point(10, 10),
			cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 0), 2, 2);
	}
	else {
		cv::putText(image, tooltip, cv::Point(10, 10),
			cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1, 4);
	}
}

void ImageConverter::closeOpencvWindows() {
	cv::destroyAllWindows();
}

cv::Mat ImageConverter::rawToDisplayMat(cv::Mat* rawMat) {
	cv::Mat image;

	cv::resize(*rawMat, image, cv::Size(OPENCV_WINDOW_MAX_WIDTH,
		imageHeight * OPENCV_WINDOW_MAX_WIDTH / imageWidth));
	
	if (showHud) {
		printInfos(image, false);
	}

	return image;
}

cv::Mat* ImageConverter::arrayToCvMat(unsigned char* data) {

	cv::Mat* image = new cv::Mat(cv::Size(imageWidth, imageHeight), CV_8UC3, *data);

	return image;
}