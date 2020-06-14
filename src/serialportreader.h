#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QByteArray>
#include <QSerialPort>
#include <QTextStream>
#include <QTimer>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    SerialPortReader(QSerialPort *serialPort, QObject *parent = nullptr);
    SerialPortReader(const QString portName, qint32 baudrate,  QObject *parent = nullptr);

signals:
    void frameChanged(QByteArray frame);
    void channelChanged(int channel, int value);
    void crcErrorCountChanged(int crcErrorCount);

private slots:
//    void handleReadyRead();
    void handleTimeout();
    void handleSimData();
    void handleError(QSerialPort::SerialPortError error);
    void onReadyRead();
    void processData(QByteArray& data);


private:
    void init();

    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;
    QTextStream m_standardOutput;

    QTimer m_timer;
    QByteArray m_frame;
    QList<int> channels;
    int numChannels;

    int m_crcErrorCount;
    int m_frameCount;


};

#endif // SERIALPORTREADER_H
