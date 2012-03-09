/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -a thermometerwatcher.h:thermometerwatcher.cpp -i types.h xml/thermometerwatcher.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <QStringListModel>

#include "characteristic.h"
#include "thermometerwatcher.h"
#include "thermometer.h"

using namespace org::bluez;

class DeviceListModel : public QStringListModel {
public:
    DeviceListModel(QObject* parent) : QStringListModel(parent)
    {
        QHash<int, QByteArray> roleNames;
        roleNames.insert(Qt::DisplayRole, "title");
        setRoleNames(roleNames);
    }
};

/*
 * Implementation of adaptor class ThermometerWatcherAdaptor
 */

ThermometerWatcherAdaptor::ThermometerWatcherAdaptor()
    : QDBusAbstractAdaptor(QCoreApplication::instance()), m_value("-1"), m_timetype(""), m_manager(0),
      m_adapter(0), m_selectedDevice(0), m_pairingDevice(0), m_deviceModel(new DeviceListModel(this)),
      m_selectedDeviceIndex(-1)
{
    // constructor
    setAutoRelaySignals(true);

    QTimer::singleShot(0, this, SLOT(delayedInitialization()));
}

ThermometerWatcherAdaptor::~ThermometerWatcherAdaptor()
{
    // destructor
    destroyDevices();
}

void ThermometerWatcherAdaptor::delayedInitialization()
{
    m_manager = new Manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH, QDBusConnection::systemBus(), this);
    setAdapter();
}

void ThermometerWatcherAdaptor::MeasurementReceived(const QVariantMap &measure)
{
    qDebug() << "Values: " << measure["Mantissa"].toInt() << measure["Exponent"].toInt() << measure["Unit"].toString();

    float value = measure["Mantissa"].toInt() * powf(10, measure["Exponent"].toInt());
    m_value = QString::number(value, 'f', 1);

    if (measure["Unit"].toString() == "Celsius")
        m_value += QString::fromUtf8("\n\u00b0C");
    else
        m_value += QString::fromUtf8("\n\u00b0F");


    if (measure.keys().contains("Time")) {
        quint64 tmp = measure["Time"].toULongLong() * 1000;
        QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(tmp);
        m_timetype = timestamp.toString("ddd MMM d yyyy hh:mm:ss");
    } else
        m_timetype = "";

    if (measure.keys().contains("Type"))
        m_timetype += QString("Type: ") + measure["Type"].toString();

    emit valueChangedSignal();
}

void ThermometerWatcherAdaptor::setDevice(int index)
{
    qWarning() << "Setting device..";
    if (index < 0 || index >= m_devices.count()) {
        setStatusMessage("Device index out of range");
        return;
    }

    Device* device = m_devices[index];
    if (m_selectedDevice == device)
        return;
    m_selectedDevice = device;
    m_selectedDeviceIndex = index;
    emit deviceChangedSignal();

    setStatusMessage("Waiting for measurements...");
    Thermometer thermometer(BLUEZ_SERVICE_NAME, device->path(), QDBusConnection::systemBus());
    thermometer.RegisterWatcher(QDBusObjectPath(COLLECTOR_OBJPATH));
}

void ThermometerWatcherAdaptor::onDeviceCreated(const QDBusObjectPath &objPath)
{
    qDebug() << "Device created: " << objPath.path();

    if (m_pairingDevice && !m_devices.contains(m_pairingDevice)) {
        // Deleting last device created but not paired, maybe some error occurred.
        m_pairingDevice->deleteLater();
    }

    m_pairingDevice = new Device(BLUEZ_SERVICE_NAME, objPath.path(),
                                QDBusConnection::systemBus(), this);

    connect(m_pairingDevice, SIGNAL(PropertyChanged(QString,QDBusVariant)),
            this, SLOT(onDevicePropertyChanged(QString,QDBusVariant)));
}

void ThermometerWatcherAdaptor::onDeviceRemoved(const QDBusObjectPath &objPath)
{
    qDebug() << "Device removed: " << objPath.path();

    foreach(Device* d, m_devices) {
        if (d->path() == objPath.path()) {
            if (d == m_selectedDevice) {
                m_selectedDevice = 0;
                m_selectedDeviceIndex = -1;
                m_value = "-1";
                setStatusMessage("No device selected");
                emit deviceChangedSignal();
            }
            m_devices.removeOne(d);
            break;
        }
    }
    m_deviceModel->setStringList(getDevicesName());
}

void ThermometerWatcherAdaptor::onDevicePropertyChanged(const QString &in0, const QDBusVariant &in1)
{
    if (in0 == "Trusted" && in1.variant().toBool()) {
        Device *d = static_cast<Device*>(sender());
        qDebug() << "Device added: " << d->path();
        if (!m_devices.contains(d) && checkServices(d)) {
            m_devices << d;
            m_deviceModel->setStringList(getDevicesName());
        }
    }
}

void ThermometerWatcherAdaptor::setAdapter()
{
    if (!m_manager) {
        qWarning() << "Invalid manager..";
        return;
    }

    if (m_adapter) {
        destroyDevices();
        delete m_adapter;
    }

    qWarning() << "Looking for adapter...";

    QDBusReply<QDBusObjectPath> obReply;
    obReply = m_manager->DefaultAdapter();
    if (!obReply.isValid()) {
        qWarning() << "Error:" << obReply.error();
        return;
    }

    qDebug() << obReply.value().path();

    m_adapter = new Adapter(BLUEZ_SERVICE_NAME, obReply.value().path(),
                        QDBusConnection::systemBus());

    connect(m_adapter, SIGNAL(DeviceCreated(QDBusObjectPath)),
                     this, SLOT(onDeviceCreated(QDBusObjectPath)));
    connect(m_adapter, SIGNAL(DeviceRemoved(QDBusObjectPath)),
            this, SLOT(onDeviceRemoved(QDBusObjectPath)));

    lookDevices();
}

bool ThermometerWatcherAdaptor::checkServices(Device* device) const
{
    QDBusReply<PropertyMap> properties = device->GetProperties();
    QVariant uuids = properties.value().value("UUIDs");

    return uuids.toStringList().contains(HEALTH_THERMOMETER_UUID, Qt::CaseInsensitive);
}

void ThermometerWatcherAdaptor::lookDevices(void)
{
    qWarning() << "Looking for devices... ";
    QDBusReply<QList<QDBusObjectPath> > slReply = m_adapter->ListDevices();

    if (!slReply.isValid()) {
        qWarning() << "Error: " << slReply.error();
        return;
    }
    m_devices.clear();

    QList<QDBusObjectPath> obpList = slReply.value();
    for (int i = 0; i < obpList.count(); i++) {
        Device* device = new Device(BLUEZ_SERVICE_NAME, obpList[i].path(),
                                    QDBusConnection::systemBus(), this);
        if (checkServices(device))
            m_devices << device;
        else
            delete device;
    }

    // Fill the device model
    QStringList list = getDevicesName();
    m_deviceModel->setStringList(list);
    qDebug() << "done! " << list.count() << " devices found.";
    setStatusMessage("No device selected");
}

void ThermometerWatcherAdaptor::destroyDevices()
{
    qDeleteAll(m_devices);
    m_devices.clear();
}

QAbstractItemModel* ThermometerWatcherAdaptor::getDeviceModel() const
{
    return m_deviceModel;
}

void ThermometerWatcherAdaptor::setStatusMessage(const QString &msg)
{
    m_timetype = msg;
    emit valueChangedSignal();
}

QStringList ThermometerWatcherAdaptor::getDevicesName()
{
    QStringList list;
    foreach(Device* d, m_devices) {
        QDBusReply<PropertyMap> properties = d->GetProperties();
        list << properties.value().value("Name").toString();
    }
    return list;
}
