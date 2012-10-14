#include "aboutauthordialog.h"
#include "ui_aboutauthordialog.h"

AboutAuthorDialog::AboutAuthorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutAuthorDialog)
{
  ui->setupUi(this);
  connect(ui->pushButton,
          SIGNAL(clicked()),
          SLOT(accept()));
}

AboutAuthorDialog::~AboutAuthorDialog()
{
  delete ui;
}
