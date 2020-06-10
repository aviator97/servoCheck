#ifndef SERVOWINDOW_H
#define SERVOWINDOW_H

#include <QTableView>
#include "mdichildbase.h"

class SerialPortReader;
class ServoModel;

class ServoWindow : public QTableView, public MdiChildBase
{
public:
    ServoWindow(SerialPortReader *serialPortReader, QWidget *parent = nullptr);
    ServoModel *getServoModel() { return servoModel; }
private:
    ServoModel *servoModel;
};

#endif // SERVOWINDOW_H
