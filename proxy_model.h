#ifndef PROXYMODEL_H
#define PROXYMODEL_H
#include <QSortFilterProxyModel>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ProxyModel(QObject* parent = 0);
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;
public slots:
    void setFilter(const QString& filter);
private:
    QString m_filter;
};

#endif // PROXYMODEL_H
