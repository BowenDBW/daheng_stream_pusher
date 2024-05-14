#pragma once

#include "camera_reader.h"

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include "ui_daheng_stream_pusher.h"

class daheng_stream_pusher : public QMainWindow
{
    Q_OBJECT

public:
    daheng_stream_pusher(QWidget *parent = nullptr);
    ~daheng_stream_pusher();

private:
    Ui::MainWidget ui;
};
