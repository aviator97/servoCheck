#include <iostream>
#include "ChannelModel.h"


ChannelModel::ChannelModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

//-----------------------------------------------------------------
int ChannelModel::rowCount(const QModelIndex & /*parent*/) const
{
    return ROWS;
}

//-----------------------------------------------------------------
int ChannelModel::columnCount(const QModelIndex & /*parent*/) const
{
    return COLS;
}

//-----------------------------------------------------------------
QVariant ChannelModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return m_gridData[index.row()][index.column()];
    }
    return QVariant();
}

//-----------------------------------------------------------------
//! [quoting mymodel_e]
bool ChannelModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        //save value from editor to member m_gridData
        m_gridData[index.row()][index.column()] = value.toString();
        //for presentation purposes only: build and emit a joined string
        QString result;
        for (int row= 0; row < ROWS; row++)
        {
            for(int col= 0; col < COLS; col++)
            {
                result += m_gridData[row][col] + ' ';
            }
        }
        emit editCompleted( result );
    }
    return true;
}
//! [quoting mymodel_e]

//-----------------------------------------------------------------
//! [quoting mymodel_f]
Qt::ItemFlags ChannelModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void ChannelModel::setChannel(int channel, int value)
{

//    std::cout << __FILE__ << ":" <<__LINE__ << " " << __FUNCTION__ << " value = " << 0.125*(value-12000) << std::endl;

    m_gridData[channel][0] = QString("%1").arg(0.125*value);
    m_gridData[channel][1] = QString("%1").arg(0.125*value - 1500.0);
    m_gridData[channel][2] = QString("%1").arg(0.125*value);
    //we identify the top left cell
    QModelIndex topLeft = createIndex(channel,0);
    QModelIndex bottomRight = createIndex(channel,1);
    //emit a signal to make the view reread identified data
    emit dataChanged(topLeft, bottomRight);
}
//! [quoting mymodel_f]
