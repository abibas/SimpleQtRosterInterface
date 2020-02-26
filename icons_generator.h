#ifndef ICONS_GENERATOR_H
#define ICONS_GENERATOR_H

#include <contact_info.h>
#include <QPixmap>
#include <QColor>
#include <QString>

class IconsGenerator
{
public:
    static QPixmap generateBigIcon(const ContactInfo& contactData);
    static QPixmap generateSmallIcon(const ContactInfo& contactData);
private:
    static QColor  getColorFromSex(const SexType sex);
    static QString getInitialsFromNames(const QString& firstName, const QString& lastName);
    static QPixmap generateIcon(const QColor color, const int iconSize, const int fontSize, const QString text);
private:
    static const int m_cSmallIconSize = 32;
    static const int m_cBigIconSize = 128;
    static const int m_cSmallIconFontSize = 10;
    static const int m_cBigIconFontSize = 40;
};

#endif // ICONS_GENERATOR_H
