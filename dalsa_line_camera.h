#pragma once

#include "image_converter.h"

#include <dalsa/sapclassbasic.h>

#include <string>
#include <queue>

/// <summary>
///     this enum tells outcome when calling setupCamera
/// </summary>
enum SetupState {
    deviceNotCreated = 0,
    bufferNotCreated,
    xFerNotCreated,
    FeatureNotCreated,
    ready
};

/// <summary>
///     This class defines how camera is connected 
///     and source image is read from da_hang camera
/// </summary>
class DalsaLineCamera
{
public:
    /// <summary>
    ///     get singleton instance of this class
    /// </summary>
    /// <returns>the only instance of this class</returns>
    static DalsaLineCamera& getInstance();

    /// <summary>
    ///     this function operate data received from camera
    /// </summary>
    /// <param name="pInfo"></param>
    static void handleCallback(SapXferCallbackInfo* pInfo);

    /// <summary>
    ///     this function will return device count total
    /// </summary>
    /// <returns>number of servers</returns>
    int getServerCount();

    /// <summary>
    ///     this function get all server names
    /// </summary>
    /// <returns>server names' vector</returns>
    std::vector<std::string>* getServerNames();

    /// <summary>
    ///     this function judges whether there are camera devices
    /// </summary>
    /// <returns></returns>
    bool isAvailable();

    /// <summary>
    ///     this function connect daheng camera
    /// </summary>
    /// <returns>feedback on to what stage is the camera connected</returns>
    int connect();

    /// <summary>
    ///     start to receive data after it is set
    /// </summary>
    /// <param name="mode"></param>
    /// <returns>is the grab begins</returns>
    bool grab();

    /// <summary>
    ///     stop receiving data
    /// </summary>
    /// <returns>is the grab ends</returns>
    bool freeze();

    /// <summary>
    ///     return its info from callback function
    /// </summary>
    /// <returns></returns>
    std::string getInfos();

private:
    /// <summary>
    ///     record device info
    /// </summary>
    std::string infoString;
    /// <summary>
    ///     signal to tell whether camera is in grabbing
    /// </summary>
    bool onGrabing = false;
    /// <summary>
    ///     signal to tell whether camera is all set and ready to grab
    /// </summary>
    bool readyToGrab = false;
    /// <summary>
    ///     to store daheng camera's index
    /// </summary>
    int deviceIndex;
    /// <summary>
    ///     to store daheng camera's name
    /// </summary>
    std::string deviceName;

    SapManager* manager = new SapManager();
    SapAcqDevice* acqDevice;
    SapLocation* location;
    static SapBuffer* sapBuffer;
    static SapTransfer* transfer;
    SapFeature* feature;

    static unsigned char* tempBuffer;

    /// <summary>
    ///     to set device features in this class
    /// </summary>
    void setDeviceName();

    /// <summary>
    ///     default constructor, not callable in singleton mode
    /// </summary>
    DalsaLineCamera() = default;
    /// <summary>
    ///     default destructor, not callable in singleton mode
    /// </summary>
    ~DalsaLineCamera() = default;
    /// <summary>
    ///     copy constructor, not callable in singleton mode
    /// </summary>
    DalsaLineCamera(const DalsaLineCamera&);
    DalsaLineCamera& operator=(const DalsaLineCamera&);
};


