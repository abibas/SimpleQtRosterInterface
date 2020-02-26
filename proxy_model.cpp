#include "proxy_model.h"
#include "contacts_tree_item.h"
#include <QString>

ProxyModel::ProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_filter(""){
}

bool ProxyModel::filterAcceptsRow(int source_row,
                                  const QModelIndex &source_parent) const{
    return true;
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    ContactsTreeItem *item = static_cast<ContactsTreeItem*>(index.internalPointer());
    if ( !item->isContact() ){
        return true;
    }

    if ( item->getDisplayData(0).toString().contains("X")){
        return true;
    }
    return false;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    return sourceModel()->headerData(section, orientation, role);
}

void ProxyModel::setFilter(const QString& filter){
    m_filter = filter;
    invalidateFilter();
}
