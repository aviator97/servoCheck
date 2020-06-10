#include <iostream>
#include "serialportreader.h"

#include <QCoreApplication>
#include <stdint.h>
#include "data.h"

uint16_t crc16(uint16_t crc, uint8_t value)
{
    const uint16_t crc_polynome = 0x1021;
    crc = crc^static_cast<uint16_t>(value) << 8;
    for (int i=0; i<8; i++)
    {
        if (crc & 0x8000)
        {
            crc = (crc << 1) ^ crc_polynome;
        }
        else
        {
            crc = (crc << 1);
        }
    }
    return crc;
}


SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent) :
    QObject(parent),
    m_serialPort(serialPort),
    m_standardOutput(stdout),
    m_crcErrorCount(0),
    m_frameCount(0)

{
    init();
}

void SerialPortReader::init()
{
    if(m_serialPort) {
        connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::onReadyRead);
        connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::handleError);
        connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);
        m_timer.start(1000);
    }
    else {
        connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleSimData);
        m_timer.start(100);
    }
}

SerialPortReader::SerialPortReader(const QString portName, qint32 baudrate, QObject *parent) :
    QObject(parent),
    m_serialPort(nullptr),
    m_standardOutput(stdout),
    m_crcErrorCount(0),
    m_frameCount(0)

{
    m_serialPort = new QSerialPort;
    const QString serialPortName =portName;
    m_serialPort->setPortName(serialPortName);

    const int serialPortBaudRate = baudrate;
    m_serialPort->setBaudRate(serialPortBaudRate);

    if (!m_serialPort->open(QIODevice::ReadOnly)) {
        m_standardOutput << QObject::tr("Failed to open port %1, error: %2")
                          .arg(serialPortName)
                          .arg(m_serialPort->errorString())
                       << endl;
        m_serialPort = nullptr;
    }

    init();
}

//void SerialPortReader::handleReadyRead()
//{
//    m_readData.append(m_serialPort->readAll());

//    if (!m_timer.isActive())
//        m_timer.start(1000);
//}

void SerialPortReader::handleTimeout()
{

    m_standardOutput << QObject::tr("Data successfully received from port %1 frames = %2 crc errors = %3 ")
                                .arg(m_serialPort->portName())
                                .arg(m_frameCount)
                                .arg(m_crcErrorCount)
                             << endl;
//    if (m_readData.isEmpty()) {
//        m_standardOutput << QObject::tr("No data was currently available "
//                                        "for reading from port %1")
//                            .arg(m_serialPort->portName())
//                         << endl;
//    } else {
//        m_standardOutput << QObject::tr("Data successfully received from port %1")
//                            .arg(m_serialPort->portName())
//                         << endl;
//        m_standardOutput << m_readData << endl;
//    }

    //    QCoreApplication::quit();
}

void SerialPortReader::handleSimData()
{
    static QByteArray data;
    static int frameSize = 21;

    static int frame = 0;
    int nFrames = sizeof(testData)/frameSize;

//    m_standardOutput << QObject::tr("add simulated data frames = %1  datasize = %2")
//                        .arg(nFrames)
//                        .arg(sizeof(testData)) << endl;

    for(int i=0; i<frameSize; i++) {
        data[i] = testData[i + frame*frameSize];
    }

    if(frame < nFrames-1) {
        frame++;
    }
    else {
        frame = 0;
    }

    processData(data);

}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;
        QCoreApplication::exit(1);
    }
}

// #define PRINT_HEX

void SerialPortReader::onReadyRead()
{
    static QByteArray data;

    data.append(m_serialPort->readAll());
    processData(data);
}

void SerialPortReader::processData(QByteArray& data)
{
#ifdef PRINT_HEX
    foreach(auto c, data) {
        unsigned char ch = c;
        m_standardOutput << QObject::tr("%1 ")
                            .arg(ch,2,16, QChar('0'));
    }
#endif

    int start = data.lastIndexOf(QByteArrayLiteral("\xA8"));
    int frameSize = 0;

    // Find startsequence aka 0xA801 bzw 0xA881 or return.
    while (true)
    {
        if (start == -1) return;
        if (data.size() < start+2)
        {
            if (start == 0) return;
            start = data.lastIndexOf(QByteArrayLiteral("\xA8"), start-1);
            continue;
        }

        if (data.at(start+1) != (char)0x01)
        {
            if (data.at(start +1) != (char)0x81)
            {
                if (start == 0) return;
                start = data.lastIndexOf(QByteArrayLiteral("\xA8"), start-1);
                continue;
            }
        }

        try {
            frameSize = static_cast<uint8_t>(data.at(start+2)) * 2 + 5;
        } catch (...) {
            continue;
        }
        if (data.size() - start < frameSize)
        {
            if (start == 0) return;

            start = data.lastIndexOf(QByteArrayLiteral("\xA8"), start-1);
            continue;
        }

        break;
    }

    QByteArray newFrame = data.mid(start, frameSize);
    data = data.mid(start+frameSize);

    unsigned char* raw = (unsigned char*)(newFrame.data());
//    int rawSize = data.size();

    uint16_t crc=0;
    for (int i=0; i<frameSize-2; i++)
    {
        crc = crc16(crc, newFrame.at(i));
    }

    uint16_t newCrc = raw[frameSize-2] << 8 | raw[frameSize-1];

//    uint16_t newCrc = ((unsigned char)(newFrame.at(frameSize-2)) << 8) | (unsigned char)(newFrame.at(frameSize-1));

    if ( newCrc != crc )
    {
        m_crcErrorCount++;
        emit crcErrorCountChanged(m_crcErrorCount);

#ifdef PRINT_HEX
        m_standardOutput << " crc err " << QObject::tr("%1 %2 : %3 %4")
                            .arg(newCrc,4,16, QChar('0'))
                            .arg(crc,4,16, QChar('0'))
                            .arg(int(newFrame.at(frameSize-2)), 2, QChar('0'))
                            .arg(int(newFrame.at(frameSize-1)), 2, QChar('0'))
                            ;

#endif
        //return;
    }
    else {
#ifdef PRINT_HEX
        m_standardOutput << " crc = " << QObject::tr("%1")
                            .arg(crc,4,16, QChar('0'));

#endif

    }

    if (m_frame != newFrame)
    {
        channels.clear();
        numChannels = raw[2];
        for(int i=0; i<numChannels; i++) {
            int idx = 3+2*i;
            int oldChannelVal = ((unsigned char)m_frame[idx] << 8 | (unsigned char)m_frame[idx+1]) & 0xFFFF;
            int newChannelVal = ((unsigned char)newFrame[idx] << 8 | (unsigned char)newFrame[idx+1]) & 0xFFFF;
            if(newChannelVal != oldChannelVal) {
                emit channelChanged(i, newChannelVal);
            }
            channels.push_back(raw[idx] << 8 | raw[idx+1]);
        }
        m_frameCount++;
        m_frame = newFrame;
        emit frameChanged(m_frame);
//        emit channelChanged(channels[2]);
    }

#ifdef PRINT_HEX
    m_standardOutput << " channels = " << QObject::tr("%1 %2")
                        .arg(numChannels)
                        .arg(channels[1]);
    m_standardOutput << endl;
#endif
}
