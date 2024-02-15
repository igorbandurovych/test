#ifndef ICONSTORAGE_H
#define ICONSTORAGE_H

#include <QHash>
#include <QPixmap>

class IconStorage
{
public:
    static IconStorage &instance();
    QPixmap operatorIcon(int mcc, int mnc) const;
    QPixmap countryIcon(const QString &code) const;

private:
    Q_DISABLE_COPY_MOVE(IconStorage)
    IconStorage();

    mutable QHash<QPair<int, int>, QPixmap> m_operatorIcons;
    mutable QHash<QString, QPixmap> m_countryIcons;
    QPixmap m_unknownIcon;
};

#endif // ICONSTORAGE_H
