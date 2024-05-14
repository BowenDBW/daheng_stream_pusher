#pragma once

#include <dalsa/SapClassBasic.h>

class ReadCallbackFactory
{
public:
	static void onGetBuffer(SapXferCallbackInfo* pInfo);
};

