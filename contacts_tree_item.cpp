#include "contacts_tree_item.h"

ContactsTreeItem::ContactsTreeItem(const QVariant displayStringData,
                                   ContactsTreeItem *parent,
                                   const bool isContact,
                                   const ContactInfo &contactData)
    : m_displayStringData(displayStringData)
    , m_parentItem(parent)
    , m_isContact(isContact)
    , m_contactData(contactData){}

ContactsTreeItem::~ContactsTreeItem(){
}

void ContactsTreeItem::appendChild(ContactsTreeItem *item){
    item->setIndex(m_childItems.size());
    m_childItems.append(std::shared_ptr<ContactsTreeItem>(item));
}

ContactsTreeItem *ContactsTreeItem::child(int row){
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row).get();
}

int ContactsTreeItem::childCount() const {
    return m_childItems.count();
}

int ContactsTreeItem::row() const {
    if (m_parentItem)
        return m_index;

    return 0;
}

int ContactsTreeItem::columnCount() const{
    return 1;
}

ContactInfo ContactsTreeItem::getContactData() const{
    return m_contactData;
}

QVariant ContactsTreeItem::getDisplayData(int) const{
    return m_displayStringData;
}

bool ContactsTreeItem::isContact() const{
    return m_isContact;
}

ContactsTreeItem *ContactsTreeItem::parentItem() const{
    return m_parentItem;
}

void ContactsTreeItem::clear(){
    m_childItems.clear();
}

void ContactsTreeItem::setIndex(const int index){
    m_index = index;
}
int ContactsTreeItem::getIndex() const{
    return m_index;
}
