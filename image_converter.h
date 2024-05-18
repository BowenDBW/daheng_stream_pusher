#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>

#include <QLabel>


class ImageConverter
{
public:

	static const int OPENCV_WINDOW_MAX_WIDTH = 1024;

	static const int DEFAULT_WIDTH = 4096;

	static const int DEFAULT_HEIGHT = 256;

	static int getWidth();

	static int getHeight();

	static std::string getFileSavePath();

	static void setOption(bool shallShowHud, bool shallSaveImage, 
		bool shallPushStream, bool shallShowExternOpencvWindow);

	static void setFps(int newFps);

	static int getSaveInterval();

	static std::string getRtmpUrl();

	static void setSaveInterval(int interval);

	static void setRtmpUrl(std::string url);

	static void setFileSavePath(std::string path);

	static void setPixmap(QLabel* label);

	static void consumeImageThread();

	static void closeOpencvWindows();

	static void produceImageData(unsigned char* newBytes);

	static void init(const int& width, const int& height);

	static void reset(const int& mode);

private:

	static std::string OPENCV_WINDOW_NAME;

	static bool showHud;

	static bool showExternOpencvWindow;

	static bool saveImage;

	static bool pushStream;

	static int fps;

	static int totalReceive;

	static int imageWidth;

	static int imageHeight;

	static int saveInterval;

	static QLabel* pixmap;

	static std::string fileSavePath;

	static std::string rtmpUrl;

	static std::mutex* mu;

	static std::condition_variable* cond;

	static std::queue<unsigned char*>* images;

	static void rtmpStreaming(cv::Mat* rawMat);

	static void localSave(cv::Mat* rawMat);

	static void showInOpencv(cv::Mat* rawMat);

	static void showInPixmap(cv::Mat* rawMat);

	static void printInfos(cv::Mat image, bool darkText);

	static cv::Mat* arrayToCvMat(unsigned char* data);

	static cv::Mat rawToDisplayMat(cv::Mat* rawMat);
};

