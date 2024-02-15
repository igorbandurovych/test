#include "IconStorage.h"
#include <QFile>

IconStorage::IconStorage() :
    m_unknownIcon{QStringLiteral(":/Icons/unknown.png")}
{
}

IconStorage &IconStorage::instance()
{
    static IconStorage instance;
    return instance;
}

QPixmap IconStorage::operatorIcon(int mcc, int mnc) const
{
    QPair<int, int> key {mcc, mnc};
    auto it = m_operatorIcons.find(key);
    if (it != m_operatorIcons.end())
        return it.value();

    auto iconPath = QStringLiteral("./Icons/%1_%2.png").arg(mcc).arg(mnc);
    if (QFile::exists(iconPath))
        return m_operatorIcons[key] = QPixmap(iconPath);

    iconPath = QStringLiteral(":/Icons/%1_%2.png").arg(mcc).arg(mnc);
    if (QFile::exists(iconPath))
        return m_operatorIcons[key] = QPixmap(iconPath);

    return m_unknownIcon;
}

QPixmap IconStorage::countryIcon(const QString &code) const
{
    auto it = m_countryIcons.find(code);
    if (it != m_countryIcons.end())
        return it.value();

    auto iconPath = QStringLiteral("./Icons/%1.png").arg(code);
    if (QFile::exists(iconPath))
        return m_countryIcons[code] = QPixmap(iconPath);

    iconPath = QStringLiteral(":/Icons/%1.png").arg(code);
    if (QFile::exists(iconPath))
        return m_countryIcons[code] = QPixmap(iconPath);

    return m_unknownIcon;
}
