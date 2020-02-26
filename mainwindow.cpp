#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "icons_generator.h"
#include "downloader.h"

#include <QString>
#include <QtConcurrent>
#include <QFutureWatcher>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_ciDialog(new ContactInfoDialog(this))
    , m_cRosterUrl("https://file.wowapp.me/owncloud/index.php/s/sGOXibS0ZSspQE8/download")
    , m_customListModel(nullptr)
    , m_rosterLocalPath("./roster.json")
    , m_customJsonProcessor()
{
    m_ui->setupUi(this);
    m_ciDialog->setWindowTitle("Info");

    connect(&m_downloader, &Downloader::updateDownloadProgress, this, &MainWindow::onUpdateProgress);
    connect(&m_downloader, &Downloader::loadingFinished, this, &MainWindow::loadingFinished);

    if (!m_customJsonProcessor.isRosterLoaded()){
        m_ui->progressBarLabel->setText("Loading Roster...");
        m_downloader.get(m_rosterLocalPath, m_cRosterUrl);
    } else {
        configureModel();
    }
}

MainWindow::~MainWindow()
{
    delete m_ciDialog;
    delete m_ui;
    delete m_customListModel;
}

void MainWindow::openUserCardWindow(const ContactInfo& contactInfo){
    m_ciDialog->setAvatar(IconsGenerator::generateBigIcon(contactInfo));
    m_ciDialog->setUsername(contactInfo.username);
    m_ciDialog->setFirstName(contactInfo.firstName);
    m_ciDialog->setLastName(contactInfo.lastName);
    m_ciDialog->setSex(SexTypeToString(contactInfo.sex));
    m_ciDialog->setLanguage(contactInfo.language);
    m_ciDialog->setCountry(contactInfo.country);
    m_ciDialog->setBirthday(contactInfo.birthday.toString());
    m_ciDialog->show();
}

void MainWindow::expandGroupOnFirstEntryCame(const QModelIndex& index){
    if (!index.isValid())
        return;

    ContactsTreeItem *item = static_cast<ContactsTreeItem*>(index.internalPointer());
    if (item->isContact()){
        return;
    }
    m_ui->contactsTreeView->expand(index);
}

void MainWindow::onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_ui->progressBar->setMaximum(bytesTotal);
    m_ui->progressBar->setValue(bytesReceived);
}

void MainWindow::loadingFinished(){
    m_ui->progressBarLabel->setText("Caching roster...");
    connect(&watcher, SIGNAL(finished()), this, SLOT(configureModel()));
    QFuture<bool> future = QtConcurrent::run(&this->m_customJsonProcessor, &CustomJsonProcessor::sortAndCacheRoster, m_rosterLocalPath);
    watcher.setFuture(future);
}

void MainWindow::configureModel(){
    m_ui->progressBar->hide();
    m_ui->progressBarLabel->hide();
    m_customListModel = new ContactsTreeModel("Contacts list", this);
    m_ui->contactsTreeView->setModel(m_customListModel);

    m_customJsonProcessor.openCachedRosterFile();

    //Filter
    connect(m_ui->filterLineEdit, SIGNAL(textChanged(const QString &)), m_customListModel, SLOT(setFilter(const QString &)));
    connect(m_ui->filterLineEdit, SIGNAL(textChanged(const QString &)), &m_customJsonProcessor, SLOT(reopenRoster()));

    connect(m_ui->contactsTreeView, SIGNAL(clicked(const QModelIndex &)), m_customListModel, SLOT(onClick(const QModelIndex &)));
    connect(m_customListModel, SIGNAL(showUserCard(const ContactInfo&)), this, SLOT(openUserCardWindow(const ContactInfo&)));
    connect(m_customListModel, SIGNAL(expandGroup(const QModelIndex&)), this, SLOT(expandGroupOnFirstEntryCame(const QModelIndex &)));
    connect(m_customListModel, SIGNAL(fetchElementsFromParser(const int)), &m_customJsonProcessor, SLOT(fetchElementsFromParser(const int)));
    connect(&m_customJsonProcessor, SIGNAL(sendData(const QVector<ContactInfo>)), m_customListModel, SLOT(elementsReceived(const QVector<ContactInfo>)));
}
