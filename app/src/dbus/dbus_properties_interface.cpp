/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp org.freedesktop.DBus.Properties.xml -p dbus_properties_interface -c DBusPropertiesInterface
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "dbus_properties_interface.h"

/*
 * Implementation of interface class DBusPropertiesInterface
 */

DBusPropertiesInterface::DBusPropertiesInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

DBusPropertiesInterface::~DBusPropertiesInterface()
{
}

