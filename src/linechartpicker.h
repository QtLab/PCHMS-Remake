#ifndef LINECHARTPICKER_H
#define LINECHARTPICKER_H

#include <qwt_plot_picker.h>

class LineChart;
class QwtPlotItem;

/**
 * @brief Class of picker for LineChart.
 *
 * Class of picker for LineChart. Can show the tooltip if there is a tooltip.
 * Can also show the suggestion of SuggestionItem.
 */
class LineChartPicker: public QwtPlotPicker
{
  Q_OBJECT
public:
  LineChartPicker(QwtPlot *plot);
  virtual bool eventFilter(QObject *, QEvent *);

private:
  /**
   * @brief Select the point at a position.
   *
   * Select the point at a position.
   * If there is no point deselect the selected point.
   */
  void select(const QPoint& pos);

  inline QwtPlot *plot() {return p;}
  inline const QwtPlot *plot() const {return p;}

  QwtPlot *p;
};

#endif // LINECHARTPICKER_H
