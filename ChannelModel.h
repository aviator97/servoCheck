#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QAbstractTableModel>
#include <QString>

const int COLS= 3;
const int ROWS= 8;


class ChannelModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ChannelModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override ;
private:
    QString m_gridData[ROWS][COLS];  //holds text entered into QTableView
signals:
    void editCompleted(const QString &);

public slots:
    void setChannel(int channel, int value);
};

#endif // MYMODEL_H
