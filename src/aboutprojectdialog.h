#ifndef ABOUTPROJECTDIALOG_H
#define ABOUTPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
  class AboutProjectDialog;
}

/**
 * @brief Class of dialog to show things about this project.
 */
class AboutProjectDialog : public QDialog
{
  Q_OBJECT
public:
  explicit AboutProjectDialog(QWidget *parent = 0);
  ~AboutProjectDialog();

private:
  Ui::AboutProjectDialog *ui;
};

#endif // ABOUTPROJECTDIALOG_H
