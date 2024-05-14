#include "camera_reader.h"

CameraReader& CameraReader::getInstance() {
    static CameraReader instance;
    return instance;
}

int CameraReader::getServerCount() {
    return manager->GetServerCount();
}

std::vector<std::string>* CameraReader::getServerNames() {
    auto* serverNames = new std::vector<std::string>();
    char* nameBuffer = new char[CORSERVER_MAX_STRLEN];
    int count = getServerCount();

    for (int i = 0; i < count; i++) {
        manager->GetServerName(i, nameBuffer);
        serverNames->push_back(std::string(nameBuffer));
    }

    delete[] nameBuffer;
    return serverNames;
}

bool CameraReader::isAvailable() {
    int count = getServerCount();
    if (count == 0) {
        return false;
    }

    std::string defaultName = getServerNames()->at(0);
    if (count == 1 && defaultName == "System") {
        return false;
    }

    return true;
}

void CameraReader::setDeviceInfo() {
    if (!isAvailable()) {
        return;
    }

    if (getServerCount() == 1) {
        deviceIndex = 0;
        deviceName = getServerNames()->at(0);
    }
    else {
        deviceIndex = 1;
        deviceName = getServerNames()->at(1);
    }
}

int CameraReader::setupCamera() {
    setDeviceInfo();
    location = new SapLocation(deviceIndex, SapManager::ResourceAcq);

    acqDevice = new SapAcqDevice(*location, FALSE);
    acqDevice->Create();

    imageBuffer = new SapBufferWithTrash(2, acqDevice);
    imageBuffer->Create();

    //transfer = new SapAcqDeviceToBuf(acqDevice, imageBuffer, *callback->onGetBuffer, this);

    return 0;
}

void CameraReader::setCallbackMethod(ReadCallbackFactory* callback) {
    this->callback = callback;
}