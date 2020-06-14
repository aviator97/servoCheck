#include <iostream>
#include "ServoModel.h"
#include <QColor>
#include <QSettings>
#include <QCoreApplication>

ServoModel::ServoModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

//-----------------------------------------------------------------
int ServoModel::rowCount(const QModelIndex & /*parent*/) const
{
    return ROWS;
}

//-----------------------------------------------------------------
int ServoModel::columnCount(const QModelIndex & /*parent*/) const
{
    return COLS;
}

//-----------------------------------------------------------------
QVariant ServoModel::data(const QModelIndex &index, int role) const
{
    if(servoData_[index.row()].channel == 0) {
        return QVariant();
    }
    if (role == Qt::DisplayRole) {
        switch(index.column()) {
        case 0 : return QVariant(servoData_[index.row()].channel  );
        case 1 : return QVariant(servoData_[index.row()].channelValue  );
        case 2 : return QVariant(servoData_[index.row()].angle  );
        case 3 : return QVariant(servoData_[index.row()].centerValue  );
        case 4 : return QVariant(servoData_[index.row()].centerAngle  );
        case 5 : return QVariant(servoData_[index.row()].upperValue  );
        case 6 : return QVariant(servoData_[index.row()].upperAngle  );
        case 7 : return QVariant(servoData_[index.row()].lowerValue  );
        case 8 : return QVariant(servoData_[index.row()].lowerAngle  );
        }
//        return m_gridData[index.row()][index.column()];
    }
    if(role == Qt::BackgroundColorRole) {
        if(index.column() == 5 ) {
            if( servoData_[index.row()].channelValue > servoData_[index.row()].upperValue ) {
                return QVariant(QColor(Qt::red));
            }
        }
        if(index.column() == 7) {
            if( servoData_[index.row()].channelValue < servoData_[index.row()].lowerValue ) {
                return QVariant(QColor(Qt::red));
            }
        }
    }
    return QVariant();
}

//-----------------------------------------------------------------
bool ServoModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        switch (index.column()) {
        case 0: servoData_[index.row()].channel = value.toInt();  break;
        case 1: servoData_[index.row()].channelValue = value.toFloat();  break;
        case 2: servoData_[index.row()].angle = value.toFloat();  break;
        case 3: servoData_[index.row()].centerValue = value.toFloat();  break;
        case 4: servoData_[index.row()].centerAngle = value.toFloat();  break;
        case 5: servoData_[index.row()].upperValue = value.toFloat();  break;
        case 6: servoData_[index.row()].upperAngle = value.toFloat();  break;
        case 7: servoData_[index.row()].lowerValue = value.toFloat();  break;
        case 8: servoData_[index.row()].lowerAngle = value.toFloat();  break;
        default:
            break;
        }
        servoData_[index.row()].updateAngle();
        writeSettings();
        writeSettings(new QSettings("Friendly5J.ini", QSettings::IniFormat));

//        if(servoData_[index.row()].channelValue) {

//        }
//        emit editCompleted( result );
    }
    return true;
}

//-----------------------------------------------------------------
Qt::ItemFlags ServoModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

QVariant ServoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("Channel");
        case 1: return tr("Value");
        case 2: return tr("Angle");
        case 3: return tr("Center");
        case 4: return tr("Center angle");
        case 5: return tr("Upper");
        case 6: return tr("Upper angle");
        case 7: return tr("Lower ");
        case 8: return tr("Lower angle");
        default: return QVariant();
        }
    }
    if (orientation == Qt::Vertical) {
        return QVariant(QString("Servo#%1").arg(section+1));
    }
    return QVariant();
}



void ServoModel::setChannel(int channel, int value)
{
//    std::cout << __FILE__ << ":" <<__LINE__ << " " << __FUNCTION__ << " value = " << 0.125*(value-12000) << std::endl;
    for(int i=0; i<ROWS; i++) {
        if(servoData_[i].channel == channel+1) {
            servoData_[i].channelValue = 0.125*value;
            servoData_[i].updateAngle();

            //we identify the top left cell
            QModelIndex topLeft = createIndex(i,1);
            QModelIndex bottomRight = createIndex(i,2);
            //emit a signal to make the view reread identified data
            emit dataChanged(topLeft, bottomRight);
            emit angleChanged(this);
        }
    }
}

void ServoModel::writeSettings(QSettings* settings)
{
    if(!settings) {
        settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    }
//    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    std::cout << __FILE__ << ":" <<__LINE__ << " " << __FUNCTION__ << " write to " << qPrintable(settings->fileName())<<  std::endl;

    for(int i=0; i<ROWS; i++) {
        settings->beginGroup(QString("Servo%1").arg(i));
        settings->setValue("channel", QVariant(servoData_[i].channel));
        settings->setValue("channelValue", QVariant(servoData_[i].channelValue));
        settings->setValue("angle", QVariant(servoData_[i].angle));
        settings->setValue("centerValue", QVariant(servoData_[i].centerValue));
        settings->setValue("centerAngle", QVariant(servoData_[i].centerAngle));
        settings->setValue("upperValue", QVariant(servoData_[i].upperValue));
        settings->setValue("upperAngle", QVariant(servoData_[i].upperAngle));
        settings->setValue("lowerValue", QVariant(servoData_[i].lowerValue));
        settings->setValue("lowerAngle", QVariant(servoData_[i].lowerAngle));
        settings->endGroup();
    }
}

void ServoModel::readSettings(QSettings* settings)
{
    if(!settings) {
        settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    }
//    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    std::cout << __FILE__ << ":" <<__LINE__ << " " << __FUNCTION__ << " read from " << qPrintable(settings->fileName())<<  std::endl;

    ServoData defaultValues;

    for(int i=0; i<ROWS; i++) {
        settings->beginGroup(QString("Servo%1").arg(i));
        servoData_[i].channel       = settings->value("channel",         defaultValues.channel).toInt();
        servoData_[i].channelValue  = settings->value("channelValue",    defaultValues.channelValue).toFloat();
        servoData_[i].angle         = settings->value("angle",           defaultValues.angle).toFloat();
        servoData_[i].centerValue   = settings->value("centerValue",     defaultValues.centerValue).toFloat();
        servoData_[i].centerAngle   = settings->value("centerAngle",     defaultValues.centerAngle).toFloat();
        servoData_[i].upperValue    = settings->value("upperValue",      defaultValues.upperValue).toFloat();
        servoData_[i].upperAngle    = settings->value("upperAngle",      defaultValues.upperAngle).toFloat();
        servoData_[i].lowerValue    = settings->value("lowerValue",      defaultValues.lowerValue).toFloat();
        servoData_[i].lowerAngle    = settings->value("lowerAngle",      defaultValues.lowerAngle).toFloat();
        settings->endGroup();
    }
}

void ServoData::updateAngle()
{
    if(channelValue > centerValue) {
        angle = centerAngle + (upperAngle - centerAngle) / (upperValue - centerValue) * (channelValue - centerValue);
    }
    else {
        angle = centerAngle - (centerAngle - lowerAngle) / (centerValue - lowerValue) * (centerValue - channelValue);
    }
}
