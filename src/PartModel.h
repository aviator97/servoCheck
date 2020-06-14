#ifndef PARTMODEL_H
#define PARTMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <PartSpec.h>
class QSettings;

class PartModel : public QAbstractTableModel
{
//    static const int COLS= 14;
    static const int ROWS= 8;

    Q_OBJECT
public:
    PartModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override ;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void writeSettings(QSettings* settings = nullptr);
    void readSettings(QSettings* settings = nullptr);
    const QVector<PartSpec*>* getPartSpec() { return &partSpec_; }

private:
    QVector<PartSpec*> partSpec_;
signals:
    void editCompleted(int partNumber);

//public slots:
//    void setChannel(int channel, int value);
};

#endif // PARTMODEL_H
