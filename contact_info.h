#ifndef CONTACT_INFO_H
#define CONTACT_INFO_H
#include <QString>
#include <QDate>

enum class SexType { FEMALE, MALE, NOT_DEFINED};
inline const char* SexTypeToString(SexType v)
{
    switch (v)
    {
        case SexType::FEMALE:   return "FEMALE";
        case SexType::MALE:     return "MALE";
        default:                return "NOT_DEFINED";
    }
}

class ContactInfo {
public:
    QString group = "";
    QString username = "";
    QString firstName = "";
    QString lastName = "";
    SexType sex = SexType::NOT_DEFINED;
    QString country = "";   //TODO: show flag if possible
    QString language = "";  //TODO: parse language2, language3 etc.
    QDate   birthday = QDate();
};

#endif // CONTACT_INFO_H
