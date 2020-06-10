#ifndef CHANNELWINDOW_H
#define CHANNELWINDOW_H

#include <QTableView>
#include "mdichildbase.h"

class SerialPortReader;

class ChannelWindow : public QTableView, public MdiChildBase
{
public:
    ChannelWindow(SerialPortReader *serialPortReader, QWidget *parent = nullptr);
};

#endif // CHANNELWINDOW_H
