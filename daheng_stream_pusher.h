#pragma once

#include "camera_reader.h"

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_daheng_stream_pusher.h"

class DahengStreamPusher : public QMainWindow
{
    Q_OBJECT

public:
    DahengStreamPusher(QWidget *parent = nullptr);
    ~DahengStreamPusher() = default;

    void onClickGrab();

    void onClickFreeze();

    void onClickConnect();

private:
    Ui::MainWidget ui;

    bool grabing = false;
};
