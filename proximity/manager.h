/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p manager.h:manager.cpp -i types.h xml/manager.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef MANAGER_H_1351015159
#define MANAGER_H_1351015159

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "types.h"

/*
 * Proxy class for interface org.bluez.Manager
 */
class OrgBluezManagerInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.bluez.Manager"; }

public:
    OrgBluezManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgBluezManagerInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QDBusObjectPath> DefaultAdapter()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("DefaultAdapter"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> FindAdapter(const QString &pattern)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(pattern);
        return asyncCallWithArgumentList(QLatin1String("FindAdapter"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetProperties()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetProperties"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void AdapterAdded(const QDBusObjectPath &adapter);
    void AdapterRemoved(const QDBusObjectPath &adapter);
    void DefaultAdapterChanged(const QDBusObjectPath &adapter);
    void PropertyChanged(const QString &name, const QDBusVariant &value);
};

namespace org {
  namespace bluez {
    typedef ::OrgBluezManagerInterface Manager;
  }
}
#endif
