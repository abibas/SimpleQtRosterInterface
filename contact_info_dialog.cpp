#include "contact_info_dialog.h"
#include "ui_contact_info_dialog.h"

ContactInfoDialog::ContactInfoDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::ContactInfoDialog){
    m_ui->setupUi(this);
}

ContactInfoDialog::~ContactInfoDialog(){
    delete m_ui;
}

void ContactInfoDialog::setAvatar(const QPixmap& avatar){
    m_ui->Avatar->setPixmap(avatar);
}

void ContactInfoDialog::setUsername(const QString& username){
    m_ui->UserName->setText(username);
}
void ContactInfoDialog::setFirstName(const QString& firstName){
    m_ui->FirstName->setText(firstName);
}
void ContactInfoDialog::setLastName(const QString& lastName){
    m_ui->LastName->setText(lastName);
}
void ContactInfoDialog::setSex(const QString& sex){
    m_ui->Sex->setText(sex);
}
void ContactInfoDialog::setCountry(const QString& country){
    m_ui->Country->setText(country);
}
void ContactInfoDialog::setLanguage(const QString& language){
    m_ui->Language->setText(language);
}
void ContactInfoDialog::setBirthday(const QString& birthday){
    m_ui->Birthday->setText(birthday);
}
