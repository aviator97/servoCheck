#ifndef SERVOMODEL_H
#define SERVOMODEL_H


#include <QAbstractTableModel>
#include <QString>

class QSettings;

struct ServoData {
    int channel;
    float channelValue;
    float angle;
    float centerValue;
    float centerAngle;
    float upperValue;
    float upperAngle;
    float lowerValue;
    float lowerAngle;
    ServoData() :
        channel(0),
        channelValue(1500.0),
        angle(0.0),
        centerValue(1500.0),
        centerAngle(0.0),
        upperValue(1900.0),
        upperAngle(45.0),
        lowerValue(1100.0),
        lowerAngle(-45.0)
    {}
    void updateAngle();
};


class ServoModel : public QAbstractTableModel
{
    static const int COLS= 9;
    static const int ROWS= 8;

    Q_OBJECT
public:
    ServoModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override ;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void writeSettings(QSettings* settings = nullptr);
    void readSettings(QSettings* settings = nullptr);
    double getAngle(int servo) { return servoData_[servo-1].angle; }

signals:
    void editCompleted(const QString &);
    void angleChanged(ServoModel* model);

public slots:
    void setChannel(int channel, int value);
private:

    ServoData servoData_[ROWS];

};
#endif // SERVOMODEL_H
