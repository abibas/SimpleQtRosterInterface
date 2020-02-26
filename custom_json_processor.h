#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include "contact_info.h"

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

enum class ItemType { Group, User };

class CustomJsonProcessor : public QObject
{
    Q_OBJECT
public:
    CustomJsonProcessor();
    ~CustomJsonProcessor();

    bool isRosterLoaded();

    bool sortAndCacheRoster(const QString& rosterPath);

    bool openCachedRosterFile();
    void closeCachedRosterFile();
public slots:
    void fetchElementsFromParser(const int count);
    void reopenRoster();
signals:
    void sendData(const QVector<ContactInfo> data);
private:
    bool readNextElement(ContactInfo& contactInfo);
    void sortRoster(QJsonArray& roster);

    bool readOriginalRoster(const QString& rosterPath, QJsonDocument& rosterDoc);
    //Reasons to write to 2 diferent files:
    //    - Normally contacts count stored seperately in UserProfile data
    //    - Roster lazy-load code cleaner
    bool writeCachedRosterSize(const int rosterSize);
    bool writeCachedRoster(const QJsonDocument& rosterDoc);

    bool openCachedRoster();
    ContactInfo parseJsonEntryToContactInfo(const QJsonObject& resultObj);
private:
    QString m_path;

    const QString m_cSortedJsonPath = "./roster_sorted.json";

    QFile m_cachedRosterFile;
    QVector<ContactInfo> m_contacts;
};
#endif // JSON_LOADER_H
