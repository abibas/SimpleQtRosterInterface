#include "custom_json_processor.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

//Ugly solution to workaround swap issue
inline void swap(QJsonValueRef v1, QJsonValueRef v2) {
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

//######################## PUBLIC METHODS ##############################
CustomJsonProcessor::CustomJsonProcessor() {
}

CustomJsonProcessor::~CustomJsonProcessor() {
    m_cachedRosterFile.close();
}

bool CustomJsonProcessor::isRosterLoaded() {
    return (QFile::exists(m_cSortedJsonPath));
}

bool CustomJsonProcessor::sortAndCacheRoster(const QString& rosterPath) {
    //TODO: Drop exception to top lvl.
    QJsonDocument doc;
    readOriginalRoster(rosterPath, doc);

    QJsonObject jsonObj = doc.object();
    QJsonArray roster = jsonObj["roster"].toArray();

    sortRoster(roster);
    writeCachedRoster(QJsonDocument(roster));

    return true;
}

bool CustomJsonProcessor::openCachedRosterFile() {
    m_cachedRosterFile.setFileName(m_cSortedJsonPath);
    if ( !m_cachedRosterFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qWarning("Couldn't open file.");
        return false;
    }
    return true;
}

void CustomJsonProcessor::closeCachedRosterFile() {
    m_cachedRosterFile.close();
}
//######################## PUBLIC METHODS END ##############################


//######################## PUBLIC SLOTS ##############################
void CustomJsonProcessor::fetchElementsFromParser(const int count) {
    QVector<ContactInfo> result;
    for(int i = 0; i < count; ++i){
        ContactInfo val;
        if (!readNextElement(val) || val.group.isEmpty()){
            break;
        }
        result.push_back(val);
    }
    emit sendData(result);
}

void CustomJsonProcessor::reopenRoster(){
    closeCachedRosterFile();
    openCachedRosterFile();
}
//######################## PUBLIC SLOTS END ##############################

//######################## PRIVATE SLOTS ##############################
//LAZY
bool CustomJsonProcessor::readNextElement(ContactInfo& contactInfo) {
    QJsonValue nextJsonElement = QJsonValue::Null;
    QByteArray nextElementByteArray = "";
    char ch = '\0';

    while (ch != '{' && !m_cachedRosterFile.atEnd()) {
        m_cachedRosterFile.read(&ch, sizeof(char));
    }

    if ('\0' != ch){
        nextElementByteArray.push_back(ch);
    }
    int bracketsCounter = 1;
    while(bracketsCounter != 0 && !m_cachedRosterFile.atEnd()) {
        m_cachedRosterFile.read(&ch, sizeof(char));
        if (ch == '{'){
            ++bracketsCounter;
        } else if (ch == '}'){
            --bracketsCounter;
        }
        nextElementByteArray.push_back(ch);
    }

    if ( !nextElementByteArray.isEmpty() ){
        QJsonObject resultObj = QJsonDocument::fromJson(nextElementByteArray).object();
        contactInfo = parseJsonEntryToContactInfo(resultObj);
        return true;
    }
    return false;
}

void CustomJsonProcessor::sortRoster(QJsonArray& roster) {
    auto comparator = [](const QJsonValue& p1, const QJsonValue& p2) -> bool {
        QJsonObject obj1 = p1.toObject();
        QJsonObject obj2 = p2.toObject();
        int group1 = obj1["groupOrder"].toInt();
        int group2 = obj2["groupOrder"].toInt();
        QJsonObject accObj1 = obj1.value(QString("account")).toObject();
        QJsonObject accObj2 = obj2.value(QString("account")).toObject();

        QString name1 = accObj1["firstName"].toString() + accObj1["lastName"].toString();
        QString name2 = accObj2["firstName"].toString() + accObj2["lastName"].toString();

        if ( group1 < group2 ){
            return true;
        }
        else if (group1 == group2) {
            if ( QString::compare(name1, name2, Qt::CaseInsensitive) < 0 ){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    };

    std::sort(roster.begin(), roster.end(), comparator);
}

bool CustomJsonProcessor::readOriginalRoster(const QString &rosterPath, QJsonDocument &rosterDoc) {
    QFile file(rosterPath);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qWarning("Couldn't open file.");
        return false;
    }
    QString val = file.readAll();
    file.close();
    rosterDoc = QJsonDocument::fromJson(val.toUtf8());
    return true;
}

bool CustomJsonProcessor::writeCachedRoster(const QJsonDocument &rosterDoc) {
    QFile sortedRosterFile(m_cSortedJsonPath);
    if (!sortedRosterFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    sortedRosterFile.write(rosterDoc.toJson());
    sortedRosterFile.close();
    return true;
}

ContactInfo CustomJsonProcessor::parseJsonEntryToContactInfo(const QJsonObject& resultObj) {
    ContactInfo result;
    result.group = resultObj.value(QString("group")).toString();

    QJsonValue account = resultObj.value(QString("account"));
    result.username  = account["username"].toString();
    result.firstName = account["firstName"].toString();
    result.lastName  = account["lastName"].toString();

    QString sexStr = account["sex"].toString().toLower();
    if ( sexStr == "male" ){
        result.sex = SexType::MALE;
    } else if ( sexStr == "female" ){
        result.sex = SexType::FEMALE;
    } else {
        result.sex = SexType::NOT_DEFINED;
    }

    result.country  = account["country"].toString();
    result.language = account["language"].toString();

    int dd = account["birthday2"].toObject()["day"].toInt();
    int mm = account["birthday2"].toObject()["month"].toInt();
    int yy = account["birthday2"].toObject()["year"].toInt();
    result.birthday = QDate(yy, mm, dd);
    return result;
}
//######################## END PRIVATE ##############################
