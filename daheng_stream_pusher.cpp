﻿#include "daheng_stream_pusher.h"
#pragma execution_character_set("utf-8") 

DahengStreamPusher::DahengStreamPusher(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    QObject::connect(ui.connectButton, &QPushButton::clicked, [&] {onClickConnect(); });
    QObject::connect(ui.grabButton, &QPushButton::clicked, [&] {onClickGrab(); });
    QObject::connect(ui.freezeButton, &QPushButton::clicked, [&] {onClickFreeze(); });
    QObject::connect(ui.scanButton, &QPushButton::clicked, [&] {restart(); });
    QObject::connect(ui.folderButton, &QPushButton::clicked, [&] {onFolderButtonClicked(); });

    QObject::connect(ui.showHudBox, &QCheckBox::clicked, [&] {onBoxClicked(); });
    QObject::connect(ui.saveBox, &QCheckBox::clicked, [&] {onBoxClicked(); });
    QObject::connect(ui.pushBox, &QCheckBox::clicked, [&] {onBoxClicked(); });
    QObject::connect(ui.opencvBox, &QCheckBox::clicked, [&] {onBoxClicked(); });

    QObject::connect(ui.intervalSpin, &QSpinBox::valueChanged, [&] {onIntervalChanged(); });
    QObject::connect(ui.serverEdit, &QLineEdit::textChanged, [&] {onRtmpUrlChanged(); });
    QObject::connect(ui.savePathEdit, &QLineEdit::textChanged, [&] {onSavePathChanged(); });

    ImageConverter::setPixmap(ui.imagePixmap);
    scanDevices();
}

void DahengStreamPusher::onClickGrab() {

    QString msg = "";
    msg.append("采集设备名称：");
    msg.append("\t");
    msg.append(QString::fromStdString(DalsaLineCamera::getInstance().getServerNames()->at(1)));
    msg.append("\t\n");
    msg.append("是否显示 HUD：");
    msg.append("\t");
    msg.append(QString::fromStdString(getTrueFalseString(ui.showHudBox->isChecked())));
    msg.append("\t\n");
    msg.append("显示Opencv：");
    msg.append("\t");
    msg.append(QString::fromStdString(getTrueFalseString(ui.opencvBox->isChecked())));
    msg.append("\t\n");
    msg.append("是否隔帧保存：");
    msg.append("\t");
    msg.append(QString::fromStdString(getTrueFalseString(ui.saveBox->isChecked())));
    msg.append("\t\n");
    msg.append("是否推流：");
    msg.append("\t");
    msg.append(QString::fromStdString(getTrueFalseString(ui.pushBox->isChecked())));
    msg.append("\t\n");
    msg.append("保存帧间隔：");
    msg.append("\t");
    msg.append(QString::number(ImageConverter::getSaveInterval()));
    msg.append("\t\n");
    msg.append("保存路径：");
    msg.append("\t");
    msg.append(QString::fromStdString(ImageConverter::getFileSavePath()));
    msg.append("\t\n");
    msg.append("RTMP地址：");
    msg.append("\t");
    msg.append(QString::fromStdString(ImageConverter::getRtmpUrl()));
    msg.append("\t\n");

    int ret = QMessageBox::information(nullptr, "运行前请务必确保参数正确!", msg,
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel) {
        return;
    }

    if (DalsaLineCamera::getInstance().grab(HandleMode::liveView)) {
        ui.showHudBox->setDisabled(true);
        ui.saveBox->setDisabled(true);
        ui.pushBox->setDisabled(true);
        ui.opencvBox->setDisabled(true);

        ui.intervalSpin->setDisabled(true);

        ui.savePathEdit->setDisabled(true);
        ui.serverEdit->setDisabled(true);

        ui.folderButton->setDisabled(true);
        ui.grabButton->setDisabled(true);
        ui.freezeButton->setEnabled(true);
    }
}

std::string DahengStreamPusher::getTrueFalseString(bool flag) {
    if (flag) {
        return "true";
    }
    else {
        return "false";
    }
}

void DahengStreamPusher::onClickFreeze() {
    
    if (DalsaLineCamera::getInstance().freeze()) {
        ui.showHudBox->setEnabled(true);
        ui.saveBox->setEnabled(true);
        ui.pushBox->setEnabled(true);
        ui.opencvBox->setEnabled(true);

        ui.intervalSpin->setEnabled(true);

        ui.savePathEdit->setEnabled(true);
        ui.serverEdit->setEnabled(true);

        ui.folderButton->setEnabled(true);
        ui.grabButton->setEnabled(true);
        ui.freezeButton->setDisabled(true);
    }

    restart();
}

void DahengStreamPusher::onClickConnect() {
    int outcome = DalsaLineCamera::getInstance().connect();
    ui.connectButton->setDisabled(true);
    ui.grabButton->setEnabled(true);
}

void DahengStreamPusher::scanDevices() {

    int count = DalsaLineCamera::getInstance().getServerCount() - 1;
    ui.deviceCountEdit->setText(QString::number(count));
    if (count != 0) {
        ui.deviceNameEdit->setText(QString::fromStdString(
            DalsaLineCamera::getInstance().getServerNames()->at(1)));
        ui.connectButton->setEnabled(true);
    }
}

void DahengStreamPusher::restart() {

    qApp->closeAllWindows();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}

void DahengStreamPusher::onBoxClicked() {
    ImageConverter::setOption(
        ui.showHudBox->isChecked(),
        ui.saveBox->isChecked(),
        ui.pushBox->isChecked(),
        ui.opencvBox->isChecked()
    );
}

void DahengStreamPusher::onIntervalChanged() {
    ImageConverter::setSaveInterval(ui.intervalSpin->value());
}

void DahengStreamPusher::onRtmpUrlChanged() {
    ImageConverter::setRtmpUrl(ui.serverEdit->text().toStdString());
}

void DahengStreamPusher::onSavePathChanged() {
    ImageConverter::setFileSavePath(ui.savePathEdit->text().toStdString());
}

void DahengStreamPusher::onFolderButtonClicked() {
    QString path = QFileDialog::getExistingDirectory();
    ui.savePathEdit->setText(path);
}