#include "daheng_stream_pusher.h"

DahengStreamPusher::DahengStreamPusher(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QObject::connect(ui.connectButton, &QPushButton::clicked, [&] {onClickConnect(); });
    QObject::connect(ui.grabButton, &QPushButton::clicked, [&] {onClickGrab(); });
    QObject::connect(ui.freezeButton, &QPushButton::clicked, [&] {onClickFreeze(); });
}

void DahengStreamPusher::onClickGrab() {
    if (DalsaLineCamera::getInstance().grab(HandleMode::opencvUi)) {
        ui.grabButton->setDisabled(true);
        ui.freezeButton->setDisabled(false);
    }
}

void DahengStreamPusher::onClickFreeze() {
    //ImageConverter::closeOpencvWindows();
    if (DalsaLineCamera::getInstance().freeze()) {
        ui.grabButton->setDisabled(false);
        ui.freezeButton->setDisabled(true);
    }
}

void DahengStreamPusher::onClickConnect() {
    int outcome = DalsaLineCamera::getInstance().connect();
}
