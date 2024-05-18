#include "dalsa_line_camera.h"

unsigned char* DalsaLineCamera::tempBuffer = nullptr;

SapBuffer* DalsaLineCamera::sapBuffer = nullptr;
SapTransfer* DalsaLineCamera::transfer = nullptr;

DalsaLineCamera& DalsaLineCamera::getInstance() {
    static DalsaLineCamera instance;
    return instance;
}

int DalsaLineCamera::getServerCount() {
    return manager->GetServerCount();
}

std::vector<std::string>* DalsaLineCamera::getServerNames() {
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

bool DalsaLineCamera::isAvailable() {
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

void DalsaLineCamera::setDeviceName() {
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

int DalsaLineCamera::connect() {
    setDeviceName();
    location = new SapLocation(deviceIndex, SapManager::ResourceAcq);
    acqDevice = new SapAcqDevice(*location, FALSE);
    if (!acqDevice->Create()) {
        return SetupState::deviceNotCreated;
    }
    sapBuffer = new SapBufferWithTrash(1, acqDevice);
    if (!sapBuffer->Create()) {
        return SetupState::bufferNotCreated;
    }
    transfer = new SapAcqDeviceToBuf(acqDevice, sapBuffer, handleCallback, this);
    if (transfer && !transfer->Create()) {
        return SetupState::xFerNotCreated;
    }

    feature = new SapFeature(acqDevice->GetLocation());
    if (feature && !*feature && !feature->Create()) {
        return SetupState::FeatureNotCreated;
    }

    ImageConverter::init(sapBuffer->GetWidth(), sapBuffer->GetHeight());

    readyToGrab = true;
    return SetupState::ready;
}

bool DalsaLineCamera::grab(const int& mode) {

    ImageConverter::reset(mode);

    if (!readyToGrab) {
        return false;
    }

    transfer->Grab();
    onGrabing = true;
    return true;
}

bool DalsaLineCamera::freeze() {
    transfer->Freeze();
    if (!transfer->Wait(500)) {
        return false;
    }
    else {
        onGrabing = false;
        return true;
    }

}

std::string DalsaLineCamera::getInfos() {
    return "";
}

void DalsaLineCamera::handleCallback(SapXferCallbackInfo* pInfo) {

    ImageConverter::setFps(
        transfer->GetFrameRateStatistics()->GetLiveFrameRate()
    );

    unsigned char* imageBuffer =
        new unsigned char[ImageConverter::getWidth() * ImageConverter::getHeight() * 3];

    sapBuffer->GetAddress((void**)&(tempBuffer));
    memcpy(imageBuffer, tempBuffer, ImageConverter::getWidth() * ImageConverter::getHeight() * 3);

    ImageConverter::produceImageData(imageBuffer);
}