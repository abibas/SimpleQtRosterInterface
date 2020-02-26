#include "icons_generator.h"
#include <QPainter>
#include <QRect>

QPixmap IconsGenerator::generateSmallIcon(const ContactInfo& contactData) {
    QColor color = getColorFromSex(contactData.sex);
    QString initials = getInitialsFromNames(contactData.firstName, contactData.lastName);
    return generateIcon(color, m_cSmallIconSize, m_cSmallIconFontSize, initials);
}

QPixmap IconsGenerator::generateBigIcon(const ContactInfo &contactData) {
    QColor color = getColorFromSex(contactData.sex);
    QString initials = getInitialsFromNames(contactData.firstName, contactData.lastName);
    return generateIcon(color, m_cBigIconSize, m_cBigIconFontSize, initials);
}

QPixmap IconsGenerator::generateIcon(const QColor color, const int iconSize, const int fontSize, const QString text){
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // +1 pixel border from top and bottom to prevent shape distortion.
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    QPixmap pixmap(iconSize+2,iconSize+2);
    pixmap.fill();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(color, 2);
    painter.setPen(pen);

    QBrush brush(color);
    painter.setBrush(brush);
    painter.drawEllipse(1, 1, iconSize, iconSize);

    painter.setPen( QColor("Black") );
    painter.setFont(QFont("Arial", fontSize));
    painter.drawText(QRect(1, 1, iconSize, iconSize), Qt::AlignCenter, text);
    return pixmap;
}

QColor IconsGenerator::getColorFromSex(const SexType sex){
    QColor color;
    //TODO MOVE TO SMART MAP.
    switch(sex){
        case SexType::MALE:{
            color.setNamedColor("#B5E6FF");
            break;
        }
        case SexType::FEMALE:{
            color.setNamedColor("#FCD0FC");
            break;
        }
        default:{
            color.setNamedColor("#E1E8ED");
            break;
        }
    }
    return color;
}

QString IconsGenerator::getInitialsFromNames(const QString& firstName, const QString& lastName){
    QString result;
    if (!firstName.isEmpty()){
        result+= firstName[0];
    }
    if (!lastName.isEmpty()){
        result+= lastName[0];
    }
    return result;
}
