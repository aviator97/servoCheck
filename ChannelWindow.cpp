#include <QHeaderView>
#include <QMdiArea>
#include <iostream>
#include "ChannelModel.h"
#include "ProgressBarDelegate.h"
#include "ChannelWindow.h"
#include "serialportreader.h"

ChannelWindow::ChannelWindow(SerialPortReader *serialPortReader, QWidget *parent) : QTableView(parent)
{
    ChannelModel *channelModel = new ChannelModel(this);
    setModel(channelModel);
    setItemDelegateForColumn(2,new ProgressBarDelegate(this));
    setWindowTitle("Channels");
    horizontalHeader()->setStretchLastSection(true);
    connect(serialPortReader, &SerialPortReader::channelChanged, channelModel, &ChannelModel::setChannel);

}
