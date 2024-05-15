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

int CameraReader::connect() {
    setDeviceInfo();
    location = new SapLocation(deviceIndex, SapManager::ResourceAcq);
    acqDevice = new SapAcqDevice(*location, FALSE);
    if (!acqDevice->Create()) {
        return SetupState::deviceNotCreated;
    }
    imageBuffer = new SapBufferWithTrash(1, acqDevice);
    if (!imageBuffer->Create()) {
        return SetupState::bufferNotCreated;
    }
    transfer = new SapAcqDeviceToBuf(acqDevice, imageBuffer, handleCallback, this);
    if (transfer && !transfer->Create()) {
        return SetupState::xFerNotCreated;
    }    

    readyToGrab = true;
    return SetupState::ready;
}

void CameraReader::handleCallback(SapXferCallbackInfo* pInfo) {
    handleMode = pInfo->GetTransfer()->GetFrameRateStatistics()->GetLiveFrameRate();

    //TODO: 获取原始数据，通过OpenCV中imread读入
}

bool CameraReader::grab(int mode) {
    if (!readyToGrab) {
        return false;
    }

    handleMode = mode;
    transfer->Grab();
    onGrabing = true;
    return true;
}

int CameraReader::freeze() {
    transfer->Freeze();
    if (!transfer->Wait(500)) {
        return false;
    }
    else {
        onGrabing = false;
        return handleMode;
    }
}

int CameraReader::handleMode = HandleMode::infoOnly;