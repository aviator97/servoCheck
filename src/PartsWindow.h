#ifndef PARTSWINDOW_H
#define PARTSWINDOW_H

#include <QTableView>
#include "mdichildbase.h"

class SerialPortReader;
class PartModel;

class PartsWindow : public QTableView, public MdiChildBase
{
public:
    PartsWindow(QWidget *parent = nullptr);
    PartModel *getPartsModel() { return partModel; }
private:
    PartModel *partModel;
};

#endif // PARTSWINDOW_H
