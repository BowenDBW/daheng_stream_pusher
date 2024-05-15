#pragma once

#include <dalsa/sapclassbasic.h>

#include <string>

/// <summary>
///     this enum tells outcome when calling setupCamera
/// </summary>
enum SetupState {
    deviceNotCreated = 0,
    bufferNotCreated,
    xFerNotCreated,
    ready
};

/// <summary>
///     this enum tells how will callback function operate camera's data
/// </summary>
enum HandleMode {
    infoOnly = 0,
    opencvUi,
    rtmp,
    rtsp
};

/// <summary>
///     This class defines how camera is connected 
///     and source image is read from da_hang camera
/// </summary>
class CameraReader
{
public:
    /// <summary>
    ///     get singleton instance of this class
    /// </summary>
    /// <returns>the only instance of this class</returns>
    static CameraReader& getInstance();

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
    bool grab(int mode);

    /// <summary>
    ///     stop receiving data
    /// </summary>
    /// <returns>is the grab ends</returns>
    int freeze();

private:
    /// <summary>
    ///     signal to tell whether camera is in grabbing
    /// </summary>
    bool onGrabing = false;
    /// <summary>
    ///     signal to tell whether camera is all set and ready to grab
    /// </summary>
    bool readyToGrab = false;
    /// <summary>
    ///     signal to tell callback fucntion how to operate the data
    /// </summary>
    static int handleMode;
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
    SapBuffer* imageBuffer;
    SapTransfer* transfer;
    SapView* view;

    /// <summary>
    ///     to set device features in this class
    /// </summary>
    void setDeviceInfo();
    
    /// <summary>
    ///     default constructor, not callable in singleton mode
    /// </summary>
    CameraReader() = default;
    /// <summary>
    ///     default destructor, not callable in singleton mode
    /// </summary>
    ~CameraReader() = default;
    /// <summary>
    ///     copy constructor, not callable in singleton mode
    /// </summary>
    CameraReader(const CameraReader&);
    CameraReader& operator=(const CameraReader&);
};

