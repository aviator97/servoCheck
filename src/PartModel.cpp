#include <iostream>
#include "PartModel.h"
#include <QColor>
#include <QSettings>
#include <QCoreApplication>

PartModel::PartModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    for(int i=0; i<ROWS; i++) {
        partSpec_.append(new PartSpec());
    }
}

//-----------------------------------------------------------------
int PartModel::rowCount(const QModelIndex & /*parent*/) const
{
    return ROWS;
}

//-----------------------------------------------------------------
int PartModel::columnCount(const QModelIndex & /*parent*/) const
{
    return PartSpec::ITEMS;
}

//-----------------------------------------------------------------
QVariant PartModel::data(const QModelIndex &index, int role) const
{
    if(partSpec_[index.row()]->servo_ == 0) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {
        case 0 : return QVariant(partSpec_[index.row()]->name_  );
        case 1 : return QVariant(partSpec_[index.row()]->servo_  );
        case 2 : return QVariant(partSpec_[index.row()]->part_  );
        case 3 : return QVariant(partSpec_[index.row()]->dim_.x()  );
        case 4 : return QVariant(partSpec_[index.row()]->dim_.y()  );
        case 5 : return QVariant(partSpec_[index.row()]->dim_.z()  );
        case 6 : return QVariant(partSpec_[index.row()]->joint_.x()  );
        case 7 : return QVariant(partSpec_[index.row()]->joint_.y()  );
        case 8 : return QVariant(partSpec_[index.row()]->joint_.z()  );
        case 9 : return QVariant(partSpec_[index.row()]->pos_.x()  );
        case 10 : return QVariant(partSpec_[index.row()]->pos_.y()  );
        case 11 : return QVariant(partSpec_[index.row()]->pos_.z()  );
        case 12 : return QVariant(partSpec_[index.row()]->axis_.x()  );
        case 13 : return QVariant(partSpec_[index.row()]->axis_.y()  );
        case 14 : return QVariant(partSpec_[index.row()]->axis_.z()  );
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------
bool PartModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        switch (index.column()) {
        case 0: partSpec_[index.row()]->name_ = value.toString();  break;
        case 1: partSpec_[index.row()]->servo_ = value.toInt();  break;
        case 2: partSpec_[index.row()]->part_ = value.toString();  break;
        case 3: partSpec_[index.row()]->dim_._v[0] = value.toFloat();  break;
        case 4: partSpec_[index.row()]->dim_._v[1] = value.toFloat();  break;
        case 5: partSpec_[index.row()]->dim_._v[2] = value.toFloat();  break;
        case 6: partSpec_[index.row()]->joint_._v[0] = value.toFloat();  break;
        case 7: partSpec_[index.row()]->joint_._v[1] = value.toFloat();  break;
        case 8: partSpec_[index.row()]->joint_._v[2] = value.toFloat();  break;
        case 9: partSpec_[index.row()]->pos_._v[0] = value.toFloat();  break;
        case 10: partSpec_[index.row()]->pos_._v[1] = value.toFloat();  break;
        case 11: partSpec_[index.row()]->pos_._v[2] = value.toFloat();  break;
        case 12: partSpec_[index.row()]->axis_._v[0] = value.toFloat();  break;
        case 13: partSpec_[index.row()]->axis_._v[1] = value.toFloat();  break;
        case 14: partSpec_[index.row()]->axis_._v[2] = value.toFloat();  break;
        default:
            break;
        }
        writeSettings();
        writeSettings(new QSettings("Friendly5J.ini", QSettings::IniFormat));
        emit editCompleted(index.row());
//        partSpec_[index.row()]->updateAngle();

//        if(partSpec_[index.row()]->channelValue) {

//        }
//        emit editCompleted( result );
    }
    return true;
}

//-----------------------------------------------------------------
Qt::ItemFlags PartModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

QVariant PartModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("Name");
        case 1: return tr("Servo");
        case 2: return tr("Type");
        case 3: return tr("length");
        case 4: return tr("width");
        case 5: return tr("height");
        case 6: return tr("joint x");
        case 7: return tr("joint y");
        case 8: return tr("joint z");
        case 9: return tr("pos x");
        case 10: return tr("pos y");
        case 11: return tr("pos z");
        case 12: return tr("axis x");
        case 13: return tr("axis y");
        case 14: return tr("axis z");
        default: return QVariant();
        }
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section+1);
    }
    return QVariant();
}

void PartModel::writeSettings(QSettings* settings)
{
    if(!settings) {
        settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    }
//    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    std::cout << __FILE__ << ":" <<__LINE__ << " " << __FUNCTION__ << " write to " << qPrintable(settings->fileName())<<  std::endl;

    for(int i=0; i<partSpec_.size(); i++) {
        settings->beginGroup(QString("Part%1").arg(i));
        settings->setValue("name", QVariant(partSpec_[i]->name_));
        settings->setValue("servo", QVariant(partSpec_[i]->servo_));
        settings->setValue("part", QVariant(partSpec_[i]->part_));
        settings->setValue("dim_x", QVariant(partSpec_[i]->dim_._v[0]));
        settings->setValue("dim_y", QVariant(partSpec_[i]->dim_._v[1]));
        settings->setValue("dim_z", QVariant(partSpec_[i]->dim_._v[2]));
        settings->setValue("joint_x", QVariant(partSpec_[i]->joint_._v[0]));
        settings->setValue("joint_y", QVariant(partSpec_[i]->joint_._v[1]));
        settings->setValue("joint_z", QVariant(partSpec_[i]->joint_._v[2]));
        settings->setValue("pos_x", QVariant(partSpec_[i]->pos_._v[0]));
        settings->setValue("pos_y", QVariant(partSpec_[i]->pos_._v[1]));
        settings->setValue("pos_z", QVariant(partSpec_[i]->pos_._v[2]));
        settings->setValue("axis_x", QVariant(partSpec_[i]->axis_._v[0]));
        settings->setValue("axis_y", QVariant(partSpec_[i]->axis_._v[1]));
        settings->setValue("axis_z", QVariant(partSpec_[i]->axis_._v[2]));
        settings->endGroup();
    }
}

void PartModel::readSettings(QSettings* settings)
{
    if(!settings) {
        settings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    }
//    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    std::cout << __FILE__ << ":" <<__LINE__ << " " << __FUNCTION__ << " read from " << qPrintable(settings->fileName())<<  std::endl;

    PartSpec defaultValues;

    for(int i=0; i<ROWS; i++) {
        settings->beginGroup(QString("Part%1").arg(i));
        partSpec_[i]->name_         = settings->value("name",            defaultValues.name_).toString();
        partSpec_[i]->servo_        = settings->value("servo",           defaultValues.servo_).toInt();
        partSpec_[i]->part_         = settings->value("part",            defaultValues.part_).toString();
        partSpec_[i]->dim_._v[0]    = settings->value("dim_x",           defaultValues.dim_._v[0]).toFloat();
        partSpec_[i]->dim_._v[1]    = settings->value("dim_y",           defaultValues.dim_._v[1]).toFloat();
        partSpec_[i]->dim_._v[2]    = settings->value("dim_z",           defaultValues.dim_._v[2]).toFloat();
        partSpec_[i]->joint_._v[0]  = settings->value("joint_x",         defaultValues.joint_._v[0]).toFloat();
        partSpec_[i]->joint_._v[1]  = settings->value("joint_y",         defaultValues.joint_._v[1]).toFloat();
        partSpec_[i]->joint_._v[2]  = settings->value("joint_z",         defaultValues.joint_._v[2]).toFloat();
        partSpec_[i]->pos_._v[0]    = settings->value("pos_x",           defaultValues.pos_._v[0]).toFloat();
        partSpec_[i]->pos_._v[1]    = settings->value("pos_y",           defaultValues.pos_._v[1]).toFloat();
        partSpec_[i]->pos_._v[2]    = settings->value("pos_z",           defaultValues.pos_._v[2]).toFloat();
        partSpec_[i]->axis_._v[0]   = settings->value("axis_x",          defaultValues.axis_._v[0]).toFloat();
        partSpec_[i]->axis_._v[1]   = settings->value("axis_y",          defaultValues.axis_._v[1]).toFloat();
        partSpec_[i]->axis_._v[2]   = settings->value("axis_z",          defaultValues.axis_._v[2]).toFloat();
        settings->endGroup();
    }
}
