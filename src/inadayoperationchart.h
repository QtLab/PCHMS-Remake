#ifndef INADAYOPERATIONCHART_H
#define INADAYOPERATIONCHART_H

#include "timelinechart.h"

#include <qwt_plot_curve.h>

class OperationInADay;

/**
 * @brief Class of chart to show detail operations in a day.
 */
class InADayOperationChart : public TimeLineChart
{
  Q_OBJECT
public:
  InADayOperationChart(QWidget *parent=NULL);
  virtual ~InADayOperationChart();

  /**
   * @brief Set the raw data of the operations.
   */
  void setOperations(OperationInADay *newOperations);

  /**
   * @brief Clear the curves.
   */
  void clear();

  /**
   * @brief Update the curves.
   */
  virtual void replot();

  virtual QSize	sizeHint() const;

private:
  /**
   * The operations.
   */
  OperationInADay *operations;

  /**
   * The curves.
   */
  QwtPlotCurve **curves;

  /**
   * Number of the curves.
   */
  int n;

  /**
   * The programs to watch.
   */
  QVector<QString> programs;

  /**
   * The interval in seconds.
   */
  int interval;

public slots:
  /**
   * @brief Set the programs.
   */
  void setPrograms(QVector<QString> newPrograms);

private slots:
  /**
   * @brief Show or hide an item.
   */
  void showItem(QwtPlotItem *item, bool on);
};

#endif // INADAYOPERATIONCHART_H
