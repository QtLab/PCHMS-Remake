#ifndef COLORMANAGEMENT_H
#define COLORMANAGEMENT_H

#include <QColor>

/**
 * @brief Namespace to manage color.
 */
namespace ColorManagement
{
  /**
   * @brief The the color of a chart.
   *
   * @param percentage The percentage of the number of the chart.
   * For example, if you have 4 colors, you'd better use 0.0, 0.25, 0.5, 0.75.
   * @return The color.
   */
  QColor getChartColor(double percentage);
};

#endif // COLORMANAGEMENT_H
