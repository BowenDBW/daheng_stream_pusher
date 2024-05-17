#pragma once

#include <string>

class CameraInfo
{

public:
	static std::string toJson();

	static void updateInfos();

	static void initInfos();
};

