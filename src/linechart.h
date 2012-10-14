#ifndef LINECHART_H
#define LINECHART_H

#include <qwt_plot_curve.h>

#include <QVector>

/**
 * @brief A class of a line chart.
 */
class LineChart : public QwtPlotCurve
{
public:
  static const int RttiLineChart = 1001;
  /**
   * @brief Constructor.
   */
  LineChart(const QString &, const QColor &);

  /**
   * @brief Set the color.
   */
  void setColor(const QColor &);

  /**
   * @brief Set the values.
   *
   * @param v The values.
   * @param t The tooptips.
   */
  void setValues(const QVector<QPointF>& v, const QVector<QString>& t);

  /**
   * @brief Set the tooltip of the whole chart.
   * @param tooltip The tooptip.
   */
  void setTooltip(QString tooltip);

  /**
   * @return Tooltip of the whole chart.
   */
  inline QString getTooltip() const
  {return tooltip;}

  /**
   * @brief Get the tooltip at given index.
   *
   * @param index The given index;
   */
  inline QString getTooptipAt(int index) const
  {
    if (index >= 0 && index < tooltips.size())
      return tooltips.at(index);
    return "";
  }

  /**
   * @brief The rtti to figure out which kind of chart it is.
   */
  virtual int rtti() const;

private:
  QVector<QString> tooltips;
  QString tooltip;
};

#endif // LINECHART_H
