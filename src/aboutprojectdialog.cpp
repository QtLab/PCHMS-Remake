#include "aboutprojectdialog.h"
#include "ui_aboutprojectdialog.h"

AboutProjectDialog::AboutProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutProjectDialog)
{
  ui->setupUi(this);
}

AboutProjectDialog::~AboutProjectDialog()
{
  delete ui;
}
