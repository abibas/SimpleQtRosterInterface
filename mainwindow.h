#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contact_info.h"
#include "contact_info_dialog.h"
#include "contacts_tree_model.h"
#include "custom_json_processor.h"
#include "downloader.h"

#include <QMainWindow>
#include <QNetworkReply>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
    class ContactInfoDialog;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void openUserCardWindow(const ContactInfo& contactInfo);
    void expandGroupOnFirstEntryCame(const QModelIndex& index);
    void onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void loadingFinished();
    void configureModel();
private:
private:
    Ui::MainWindow *m_ui;
    ContactInfoDialog *m_ciDialog;
    const QString m_cRosterUrl;
    ContactsTreeModel *m_customListModel;
    const QString m_rosterLocalPath;
    CustomJsonProcessor m_customJsonProcessor;

    Downloader m_downloader;
    QFutureWatcher<bool> watcher;
};
#endif // MAINWINDOW_H
