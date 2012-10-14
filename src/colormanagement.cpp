#include "colormanagement.h"

#include "algorithm.h"

QColor ColorManagement::getChartColor(double percentage)
{
  int r, g, b;
  Algorithm::HSL2RGB(percentage, 1.0, 0.5, r, g, b);
  return QColor(r, g, b);
}
