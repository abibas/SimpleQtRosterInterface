#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class Downloader: public QObject
{
    Q_OBJECT
    using BaseClass = QObject;

public:
    explicit Downloader(QObject* parent = nullptr);
    ~Downloader();
    bool get(const QString& targetPath, const QUrl& url);
private slots:
    void onReadyRead();
    void onReply(QNetworkReply* reply);
signals:
    void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void loadingFinished();
private:
    QNetworkReply* m_currentReply;
    QFile* m_file;
    QNetworkAccessManager m_manager;
};

#endif // DOWNLOADER_H
