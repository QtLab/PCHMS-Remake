#ifndef PROGRAMWIDGET_H
#define PROGRAMWIDGET_H

#include <QWidget>

#include <QStandardItemModel>
#include "statistic.h"

namespace Ui {
  class ProgramWidget;
}

/**
 * @brief Class of widget to show the statistic of a program.
 */
class ProgramWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ProgramWidget(Statistic::StatisticUnit *statistic,
                         QWidget *parent = 0);
  ~ProgramWidget();

  virtual void setVisible(bool visible);

private:
  Ui::ProgramWidget *ui;

  /**
   * The statistic.
   */
  Statistic::StatisticUnit *statistic;

  /**
   * The model.
   */
  QStandardItemModel *model;

  /**
   * The program.
   */
  QString program;

  /**
   * @brief Recalculate the charts.
   */
  void recalculate();
};

#endif // PROGRAMWIDGET_H
