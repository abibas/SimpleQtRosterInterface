#ifndef CUSTOMLISTMODEL_H
#define CUSTOMLISTMODEL_H

#include "contacts_tree_item.h"
#include <QAbstractTableModel>
#include <QList>
#include <QStandardItem>
#include <QMouseEvent>

class ContactsTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ContactsTreeModel(const QString &data, QObject *parent = nullptr);
    ~ContactsTreeModel();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

public slots:
    void onClick(const QModelIndex &index);
    void elementsReceived(const QVector<ContactInfo> elements);
    void setFilter(const QString& filter);

signals:
    void showUserCard(const ContactInfo& contactInfo);
    void expandGroup(const QModelIndex& index);
    void fetchElementsFromParser(const int count);

private:
    void addItem(ContactsTreeItem *parent, ContactsTreeItem *children);
    QPixmap generateLogo(SexType sex, const QString& firstName, const QString& lastName) const;

private:
    ContactsTreeItem *m_invisibleRootItem;
    ContactsTreeItem *m_last_group;
    bool m_is_all_elements_loaded = false;
    const int m_elementsChunkSize = 10;
    QString m_filter;
};

#endif // CUSTOMLISTMODEL_H
