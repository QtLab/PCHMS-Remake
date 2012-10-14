#ifndef ABOUTAUTHORDIALOG_H
#define ABOUTAUTHORDIALOG_H

#include <QDialog>

namespace Ui {
  class AboutAuthorDialog;
}

/**
 * @brief Class of dialog to show things about author.
 *
 * Class of dialog to show things about author.
 * Currently, here is only one author struggling-.-
 */
class AboutAuthorDialog : public QDialog
{
  Q_OBJECT
public:
  explicit AboutAuthorDialog(QWidget *parent = 0);
  ~AboutAuthorDialog();

private:
  Ui::AboutAuthorDialog *ui;
};

#endif // ABOUTAUTHORDIALOG_H
