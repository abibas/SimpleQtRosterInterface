#include "contact_info_form.h"
#include "ui_contact_info_form.h"

ContactInfoForm::ContactInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactInfoForm)
{
    ui->setupUi(this);
}

ContactInfoForm::~ContactInfoForm()
{
    delete ui;
}
