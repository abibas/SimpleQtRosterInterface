#ifndef CONTACT_INFO_FORM_H
#define CONTACT_INFO_FORM_H

#include <QWidget>

namespace Ui {
class ContactInfoForm;
}

class ContactInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit ContactInfoForm(QWidget *parent = nullptr);
    ~ContactInfoForm();

private:
    Ui::ContactInfoForm *ui;
};

#endif // CONTACT_INFO_FORM_H
