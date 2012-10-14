#ifndef INADAYWIDGET_H
#define INADAYWIDGET_H

#include <QMainWindow>

class OperationInADay;

namespace Ui {
  class InADayWidget;
}

/**
 * @brief Class of widget to show the operations in a day.
 */
class InADayWidget : public QMainWindow
{
  Q_OBJECT
public:
  explicit InADayWidget(OperationInADay *operations,
                        QWidget *parent = 0);
  ~InADayWidget();

  virtual void setVisible(bool visible);

private:
  Ui::InADayWidget *ui;

  /**
   * All of the operations of the day.
   */
  OperationInADay *operations;

private slots:
  /**
   * @brief Slot to be called when the selection of programs changed.
   */
  void selectionChanged();

  /**
   * @brief Slot to be called to show the list of operations.
   *
   * Slot to be called to show the list of operations.
   * The list will cost much memory, so I didn't put it in this widget.
   */
  void showOperationList();
};

#endif // INADAYWIDGET_H
