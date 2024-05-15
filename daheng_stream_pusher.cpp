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
    if (CameraReader::getInstance().grab(0)) {
        ui.grabButton->setDisabled(true);
        ui.freezeButton->setDisabled(false);
    }
}

void DahengStreamPusher::onClickFreeze() {
    QMessageBox::information(nullptr, "callback", QString::number(CameraReader::getInstance().freeze()));
    //if (CameraReader::getInstance().freeze()) {
        ui.grabButton->setDisabled(false);
        ui.freezeButton->setDisabled(true);
    //}
}

void DahengStreamPusher::onClickConnect() {
    int outcome = CameraReader::getInstance().connect();
}
