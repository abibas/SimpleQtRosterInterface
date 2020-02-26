#ifndef CONTACT_INFO_DIALOG_H
#define CONTACT_INFO_DIALOG_H

#include <QDialog>

namespace Ui {
class ContactInfoDialog;
}

class ContactInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContactInfoDialog(QWidget *parent = nullptr);
    ~ContactInfoDialog();

    void setAvatar(const QPixmap& avatar);
    void setUsername(const QString& username);
    void setFirstName(const QString& firstName);
    void setLastName(const QString& lastName);
    void setSex(const QString& sex);
    void setCountry(const QString& country);
    void setLanguage(const QString& language);
    void setBirthday(const QString& birthday);

private:
    Ui::ContactInfoDialog *m_ui;
};

#endif // CONTACT_INFO_DIALOG_H
