#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <qwt_plot_histogram.h>

/**
 * @brief Class of histogram.
 */
class Histogram : public QwtPlotHistogram
{
public:
  Histogram(const QString& title=QString::null,
            const QColor& symbolColor=QColor(0, 0, 0));
};

#endif // HISTOGRAM_H
