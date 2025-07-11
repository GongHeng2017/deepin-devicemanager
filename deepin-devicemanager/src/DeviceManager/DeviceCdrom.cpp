// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// 项目自身文件
#include "DeviceCdrom.h"
#include "DDLog/DDLog.h"

using namespace DDLog;

DeviceCdrom::DeviceCdrom()
    : DeviceBaseInfo()
    , m_Type("")
    , m_BusInfo("")
    , m_Capabilities("")
    , m_MaxPower("")
    , m_Speed("")
{
    qCDebug(appLog) << "DeviceCdrom constructor initialized";
    // 初始化可显示属性
    initFilterKey();
}

bool DeviceCdrom::setInfoFromLshw(const QMap<QString, QString> &mapInfo)
{
    qCDebug(appLog) << "Start setting CDROM info from lshw";

    // 通过总线信息判断是否是同一台设备
    if (!matchToLshw(mapInfo)) {
        qCDebug(appLog) << "Device not matched in lshw info, returning false.";
        return false;
    }

    // 获取设备的基本信息
    setAttribute(mapInfo, "product", m_Name, false);
    setAttribute(mapInfo, "vendor", m_Vendor, false);
    setAttribute(mapInfo, "", m_Type);
    setAttribute(mapInfo, "version", m_Version, false);
    setAttribute(mapInfo, "bus info", m_BusInfo, false);
    setAttribute(mapInfo, "capabilities", m_Capabilities, false);
    setAttribute(mapInfo, "", m_Driver);
    setAttribute(mapInfo, "", m_MaxPower);
    setAttribute(mapInfo, "", m_Speed);
    qCDebug(appLog) << "Basic attributes set from Lshw.";

    // 获取其他设备信息
    getOtherMapInfo(mapInfo);
    qCDebug(appLog) << "Finished setting CDROM info from lshw";
    return true;
}

TomlFixMethod DeviceCdrom::setInfoFromTomlOneByOne(const QMap<QString, QString> &mapInfo)
{
    qCDebug(appLog) << "DeviceCdrom::setInfoFromTomlOneByOne started.";
    TomlFixMethod ret = TOML_None;
    ret = setTomlAttribute(mapInfo, "Model", m_Type);
    ret = setTomlAttribute(mapInfo, "Bus Info", m_BusInfo);
    ret = setTomlAttribute(mapInfo, "Capabilities", m_Capabilities);
    ret = setTomlAttribute(mapInfo, "Maximum Power", m_MaxPower);
    ret = setTomlAttribute(mapInfo, "Speed", m_Speed);
    qCDebug(appLog) << "Toml attributes set.";
//3. 获取设备的其它信息
    getOtherMapInfo(mapInfo);
    qCDebug(appLog) << "DeviceCdrom::setInfoFromTomlOneByOne finished, return: " << ret;
    return ret;
}

void DeviceCdrom::setInfoFromHwinfo(const QMap<QString, QString> &mapInfo)
{
    qCDebug(appLog) << "Start setting CDROM info from hwinfo";

    // 获取设备的基本信息
    setAttribute(mapInfo, "Device", m_Name);
    setAttribute(mapInfo, "Vendor", m_Vendor);
    setAttribute(mapInfo, "Model", m_Type);
    setAttribute(mapInfo, "Revision", m_Version);
    setAttribute(mapInfo, "SysFS BusID", m_BusInfo);
    setAttribute(mapInfo, "", m_Capabilities);
    setAttribute(mapInfo, "Driver", m_Driver);
    setAttribute(mapInfo, "", m_MaxPower);
    setAttribute(mapInfo, "Speed", m_Speed);
    qCDebug(appLog) << "Basic attributes set from Hwinfo.";

    setAttribute(mapInfo, "Module Alias", m_Modalias);
    setAttribute(mapInfo, "VID_PID", m_VID_PID);
    m_PhysID = m_VID_PID;

    // 获取映射到 lshw设备信息的 关键字
    setHwinfoLshwKey(mapInfo);

    // 获取其他设备信息
    getOtherMapInfo(mapInfo);
    qCDebug(appLog) << "DeviceCdrom::setInfoFromHwinfo finished.";
}

const QString &DeviceCdrom::name()const
{
    // qCDebug(appLog) << "DeviceCdrom::name called, returning: " << m_Name;
    return m_Name;
}

const QString &DeviceCdrom::vendor() const
{
    // qCDebug(appLog) << "DeviceCdrom::vendor called, returning: " << m_Vendor;
    return m_Vendor;
}

const QString &DeviceCdrom::driver()const
{
    // qCDebug(appLog) << "DeviceCdrom::driver called, returning: " << m_Driver;
    return m_Driver;
}

QString DeviceCdrom::subTitle()
{
    // qCDebug(appLog) << "DeviceCdrom::subTitle called, returning: " << m_Name;
    return m_Name;
}

const QString DeviceCdrom::getOverviewInfo()
{
    // qCDebug(appLog) << "DeviceCdrom::getOverviewInfo called, returning: " << m_Name;
    return m_Name;
}


void DeviceCdrom::initFilterKey()
{
    qCDebug(appLog) << "DeviceCdrom::initFilterKey called.";
    // 添加可显示的属性
    addFilterKey(QObject::tr("Serial ID"));
    addFilterKey(QObject::tr("Driver Modules"));
    addFilterKey(QObject::tr("Device File"));
    addFilterKey(QObject::tr("Device Files"));
    addFilterKey(QObject::tr("Device Number"));
    // addFilterKey(QObject::tr("Module Alias"));
    addFilterKey(QObject::tr("Config Status"));
    addFilterKey(QObject::tr("Application"));
    // addFilterKey(QObject::tr("physical id"));

    addFilterKey(QObject::tr("status"));
    addFilterKey(QObject::tr("logical name"));
//    addFilterKey(QObject::tr("bus info"));
    addFilterKey(QObject::tr("ansiversion"));
    qCDebug(appLog) << "Filter keys initialized.";
}

void DeviceCdrom::loadBaseDeviceInfo()
{
    qCDebug(appLog) << "DeviceCdrom::loadBaseDeviceInfo called.";
    // 添加基本信息
    addBaseDeviceInfo(tr("Name"), m_Name);
    addBaseDeviceInfo(tr("Vendor"), m_Vendor);
    addBaseDeviceInfo(tr("Model"), m_Type);
    addBaseDeviceInfo(tr("Version"), m_Version);
    addBaseDeviceInfo(tr("Bus Info"), m_BusInfo);
    addBaseDeviceInfo(tr("Capabilities"), m_Capabilities);
    addBaseDeviceInfo(tr("Driver"), m_Driver);
    addBaseDeviceInfo(tr("Maximum Power"), m_MaxPower);
    addBaseDeviceInfo(tr("Speed"), m_Speed);
    qCDebug(appLog) << "Base device info loaded.";
}

void DeviceCdrom::loadOtherDeviceInfo()
{
    qCDebug(appLog) << "DeviceCdrom::loadOtherDeviceInfo called.";
    addOtherDeviceInfo(tr("Module Alias"), m_Modalias);
    addOtherDeviceInfo(tr("Physical ID"), m_PhysID);
    // 将QMap<QString, QString>内容转存为QList<QPair<QString, QString>>
    mapInfoToList();
    qCDebug(appLog) << "Other device info loaded.";
}

void DeviceCdrom::loadTableData()
{
    qCDebug(appLog) << "DeviceCdrom::loadTableData called.";
    // 加载表格内容
    QString tName = m_Name;

    if (!available()) {
        qCDebug(appLog) << "Device not available, updating tName.";
        tName = "(" + tr("Unavailable") + ") " + m_Name;
    }

    if (!enable()) {
        qCDebug(appLog) << "Device disabled, updating tName.";
        tName = "(" + tr("Disable") + ") " + m_Name;
    }

    m_TableData.append(tName);
    m_TableData.append(m_Vendor);
    m_TableData.append(m_Type);
    qCDebug(appLog) << "Table data loaded.";
}

