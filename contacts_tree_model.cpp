#include "contacts_tree_model.h"
#include "custom_json_processor.h"
#include "icons_generator.h"
#include <QVariant>
#include <QPainter>
#include <QThread>

//######################## PUBLIC METHODS ##############################
ContactsTreeModel::ContactsTreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
    , m_last_group(nullptr) {
    m_invisibleRootItem = new ContactsTreeItem(data);
}


ContactsTreeModel::~ContactsTreeModel() {
//    delete m_last_group;
//    delete m_invisibleRootItem;
}

int ContactsTreeModel::rowCount(const QModelIndex &parent) const {
    ContactsTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_invisibleRootItem;
    else
        parentItem = static_cast<ContactsTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ContactsTreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<ContactsTreeItem*>(parent.internalPointer())->columnCount();
    return m_invisibleRootItem->columnCount();
}


QVariant ContactsTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole) {
        ContactsTreeItem *item = static_cast<ContactsTreeItem*>(index.internalPointer());
        if (!item->isContact()){
            return QVariant();
        }
        ContactInfo contactData = item->getContactData();

        return IconsGenerator::generateSmallIcon(contactData);
    } else if (role == Qt::DisplayRole) {
        ContactsTreeItem *item = static_cast<ContactsTreeItem*>(index.internalPointer());
        return item->getDisplayData(0);
    } else {
        return QVariant();
    }
}

Qt::ItemFlags ContactsTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant ContactsTreeModel::headerData(int, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_invisibleRootItem->getDisplayData(0);

    return QVariant();
}

QModelIndex ContactsTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ContactsTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_invisibleRootItem;
    else
        parentItem = static_cast<ContactsTreeItem*>(parent.internalPointer());

    ContactsTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ContactsTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    ContactsTreeItem *childItem = static_cast<ContactsTreeItem*>(index.internalPointer());
    ContactsTreeItem *parentItem = childItem->parentItem();

    if (parentItem == nullptr){
        return QModelIndex();
    }
    if (parentItem == m_invisibleRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


bool ContactsTreeModel::canFetchMore(const QModelIndex&) const {
    return !m_is_all_elements_loaded;
}

void ContactsTreeModel::fetchMore(const QModelIndex&) {
    emit fetchElementsFromParser(m_elementsChunkSize);
}
//######################## PUBLIC END ##############################

//######################## PUBLIC SLOTS ##############################
void ContactsTreeModel::onClick(const QModelIndex &index){
    if (!index.isValid())
        return;

    ContactsTreeItem *item = static_cast<ContactsTreeItem*>(index.internalPointer());
    if (!item->isContact()){
        return;
    }

    emit showUserCard(item->getContactData());
}

void ContactsTreeModel::elementsReceived(const QVector<ContactInfo> elements){
    if(elements.size() == 0){
        m_is_all_elements_loaded = true;
        return;
    }

    bool isItemAdded = false;
    for(auto& val: elements){
        QString fullName = val.firstName + " " + val.lastName;

        if ( nullptr == m_last_group || val.group != m_last_group->getContactData().group){
            m_last_group = new ContactsTreeItem(val.group, m_invisibleRootItem, false, val);
            addItem(m_invisibleRootItem, m_last_group);
            //emit expandGroup(createIndex(m_last_group->row(), 0, m_last_group));
        }

        if ( !fullName.contains(m_filter) ){
            continue;
        }

        if (!isItemAdded){
            isItemAdded = true;
        }
        //   WORKAROUND - flat structure.
        //   TODO!!! Strange behavior of the canFetchMore and fetchMore, for the cases when 1 more nested level exists
        //   MORE TIME TO BE SPENT TO INVESTIGATE. For now tree functionality disabled!!!!!!!!!!!!
        //addItem(m_last_group, new ContactsTreeItem({QString(val.firstName + " " + val.lastName)}, m_last_group, true, val));
        addItem(m_invisibleRootItem, new ContactsTreeItem({QString(val.firstName + " " + val.lastName)}, m_invisibleRootItem, true, val));
    }

    if (!isItemAdded){
        emit fetchElementsFromParser(m_elementsChunkSize);
    }
}


void ContactsTreeModel::setFilter(const QString& filter){
    m_filter = filter;
    beginResetModel();
    m_invisibleRootItem->clear();
    m_is_all_elements_loaded = false;
    m_last_group = nullptr;
    endResetModel();
}
//###################### PUBLIC SLOTS END ############################

//######################## PRIVATE METHODS ##############################
void ContactsTreeModel::addItem(ContactsTreeItem *parent, ContactsTreeItem *children) {
    QModelIndex idx = createIndex(parent->row(), 0, parent);
    if (!idx.isValid()) {
        return;
    }

    int childCount = parent->childCount();
    beginInsertRows(idx, childCount, childCount+10);
    parent->appendChild(children);
    endInsertRows();

    emit layoutChanged(QList<QPersistentModelIndex>{idx});
}
//######################## PRIVATE METHODS END ##############################

