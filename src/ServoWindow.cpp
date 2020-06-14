#include <QHeaderView>
#include <QMdiArea>
#include <iostream>
#include "ServoModel.h"
#include "ServoWindow.h"
#include "serialportreader.h"

ServoWindow::ServoWindow(SerialPortReader *serialPortReader, QWidget *parent) : QTableView(parent)
{
    // servo configuration table

    servoModel = new ServoModel(this);
    servoModel->readSettings();
    setModel(servoModel);
    setWindowTitle("Servo Limits");
    horizontalHeader()->setStretchLastSection(true);
    connect(serialPortReader, &SerialPortReader::channelChanged, servoModel, &ServoModel::setChannel);
}
