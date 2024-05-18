#pragma once

#include "dalsa_line_camera.h"
#include "ini_parser.h"

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include<QFileDialog>

#include "ui_daheng_stream_pusher.h"

class DahengStreamPusher : public QMainWindow
{
    Q_OBJECT

public:
    DahengStreamPusher(QWidget *parent = nullptr);
    ~DahengStreamPusher() = default;

private:
    Ui::MainWidget ui;

    bool connected = false;

    bool grabing = false;

    std::string getTrueFalseString(bool flag);

    void init();
    
    void onClickGrab();

    void onClickFreeze();

    void onClickConnect();

    void scanDevices();

    void restart();

    void onBoxClicked();

    void onFolderButtonClicked();

    void onIntervalChanged();

    void onRtmpUrlChanged();

    void onSavePathChanged();
};
