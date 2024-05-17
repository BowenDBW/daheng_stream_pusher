#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>

/// <summary>
///     this enum tells how will callback function operate camera's data
/// </summary>
enum HandleMode {
	infoOnly = 0,
	opencvUi,
	localSave,
	rtmp,
};

class ImageConverter
{
public:

	static const int OPENCV_WINDOW_MAX_WIDTH = 1024;

	static const int DEFAULT_WIDTH = 4096;

	static const int DEFAULT_HEIGHT = 256;

	static int getWidth();

	static int getHeight();

	static void setFps(int newFps);

	static void consumeImageThread();

	static void closeOpencvWindows();

	static void produceImageData(unsigned char* newBytes);

	static void init(const int& width, const int& height);

	static void reset(const int& mode);

private:

	static std::string OPENCV_WINDOW_NAME;
	/// <summary>
	///     signal to tell callback fucntion how to operate the data
	/// </summary>
	static int handleMode;

	static int fps;

	static int totalReceive;

	static int imageWidth;

	static int imageHeight;

	static int opencvWindowScale;

	static int opencvWindowBarWidth;

	static std::mutex* mu;

	static std::condition_variable* cond;

	static std::queue<unsigned char*>* images;

	static void rtmpStreaming(unsigned char* data);

	static void localSave(unsigned char* data);

	static void showInOpencv(unsigned char* data);

	static void infoOnly();

	static void printInfos(cv::Mat image);

	static cv::Mat arrayToCvMat(unsigned char* data);
};

