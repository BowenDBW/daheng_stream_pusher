#pragma once

#include "read_callback_factory.h"

#include <dalsa/sapclassbasic.h>

#include <string>

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
    /// <returns></returns>
    static CameraReader& getInstance();

    /// <summary>
    ///     this function will return device count total
    /// </summary>
    /// <returns></returns>
    int getServerCount();

    std::vector<std::string>* getServerNames();

    bool isAvailable();

    int setupCamera();

    enum setupState {
        deviceNotCreated = 0,
        bufferNotCreated,
        xFerNotCreated,
        ready
    };

private:
    int deviceIndex;

    std::string deviceName;

    SapAcqDevice* acqDevice;

    SapLocation* location;

    SapBuffer* imageBuffer;

    SapTransfer* transfer;

    ReadCallbackFactory* callback;

    void setDeviceInfo();

    void setCallbackMethod(ReadCallbackFactory* callback);

    SapManager* manager = new SapManager();
    //SapAcquisition *acq;
    //SapAcqDevice *acqDevice;
    //SapBufferWithTrash *buffers;
    //SapTransfer *acqToBuf = new SapAcqToBuf(acq, buffers);
    //SapTransfer *acqDeviceToBuf = new SapAcqDeviceToBuf(acqDevice, buffers);
    //SapTransfer* Xfer = NULL;
    
    //SapLocation *loc = new SapLocation(acqServerName, acqDeviceNumber);

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

