#ifndef CONTACTSTREEITEM_H
#define CONTACTSTREEITEM_H
#include "contact_info.h"
#include <QVariant>
#include <QVector>
#include <memory>

class ContactsTreeItem
{
public:
    explicit ContactsTreeItem(const QVariant displayStringData,
                              ContactsTreeItem *parentItem = nullptr,
                              const bool isContact = false,
                              const ContactInfo &contactData = ContactInfo());
    ~ContactsTreeItem();

    void appendChild(ContactsTreeItem *child);
    ContactsTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    ContactInfo getContactData() const;
    QVariant getDisplayData(int column) const;
    bool isContact() const;
    int row() const;
    ContactsTreeItem *parentItem() const;
    void clear();
    void setIndex(const int index);
    int getIndex() const;

private:
    QVariant m_displayStringData;
    ContactsTreeItem *m_parentItem;
    QVector<std::shared_ptr<ContactsTreeItem>> m_childItems;
    int m_index;
    bool m_isContact;
    ContactInfo m_contactData;
};

#endif // CONTACTSTREEITEM_H
