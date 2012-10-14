#include "mousespectrogram.h"

#include <qwt_color_map.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include "statistic.h"
#include "operationinaday.h"
#include "timelinechart.h"

/**
 * @brief Class of color map.
 */
class ColorMap : public QwtLinearColorMap
{
public:
  ColorMap();
};

ColorMap::ColorMap() :
    QwtLinearColorMap(QColor(255, 255, 255, 0), Qt::red)
{
  addColorStop(0.001, QColor(255, 255, 255, 0));
  addColorStop(0.002, Qt::cyan);
  addColorStop(0.3, Qt::green);
  addColorStop(0.4, Qt::yellow);
  addColorStop(0.9, Qt::red);
}

const int VALUE_TO_CHANGE2[31][31] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4, 4, 5, 4, 4, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 6, 8, 6, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4, 5, 8, 16, 8, 5, 4, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 6, 8, 6, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4, 4, 5, 4, 4, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

/**
 * @brief Class of raster data for the spectrogram.
 */
class RasterData : public QwtRasterData
{
public:
  RasterData(MouseSpectrogram *spectrogram);

  /**
   * @brief Clear the data.
   */
  inline void clear()
  {
    memset(d, 0, Statistic::SCREEN_SIZE * Statistic::SCREEN_SIZE * sizeof(int));
    max = 0;
  }

  /**
   * @brief Get the max number of the data.
   */
  inline int getMax() const
  {return max;}

  /**
   * @brief Add one value to the data.
   */
  inline void addOne(int x, int y)
  {
    x = qBound(0, x, Statistic::SCREEN_SIZE - 1);
    y = qBound(0, y, Statistic::SCREEN_SIZE - 1);
    if (x >=0 && x <= Statistic::SCREEN_SIZE &&
        y >=0 && y <= Statistic::SCREEN_SIZE)
    {
      int xFrom = x >= 15 ? x - 15 : 0;
      int xTo = x < Statistic::SCREEN_SIZE - 15 ? x + 15 : Statistic::SCREEN_SIZE - 1;
      int yFrom = y >= 15 ? y - 15 : 0;
      int yTo = y < Statistic::SCREEN_SIZE - 15 ? y + 15 : Statistic::SCREEN_SIZE - 1;
      for (int i = xFrom; i <= xTo;++i)
        for (int j = yFrom;j <= yTo;++j)
        {
          d[i][j] += VALUE_TO_CHANGE2[i + 15 - x][j + 15 - y];
          max = qMax(max, d[i][j]);
        }
    }
  }

  /**
   * @brief Flush the data.
   */
  void flush();

  virtual double value(double x, double y) const;

private:
  int d[Statistic::SCREEN_SIZE][Statistic::SCREEN_SIZE];
  MouseSpectrogram *s;
  int max;
};

RasterData::RasterData(MouseSpectrogram *spectrogram) :
    s(spectrogram),
    max(0)
{
  memset(d, 0, Statistic::SCREEN_SIZE * Statistic::SCREEN_SIZE * sizeof(int));
  setInterval(Qt::XAxis,
              QwtInterval(0.0, Statistic::SCREEN_SIZE));
  setInterval(Qt::YAxis,
              QwtInterval(0.0, Statistic::SCREEN_SIZE));

  s->setAxisScale(QwtPlot::xBottom, 0, Statistic::SCREEN_SIZE);
  s->setAxisScale(QwtPlot::yLeft, Statistic::SCREEN_SIZE, 0);
  s->setAxisScale(QwtPlot::yRight, 0.0, 1.0);
}

void RasterData::flush()
{
  QwtInterval zInterval(0.0, qMax(qLn(max), 1.0));
  setInterval(Qt::ZAxis, zInterval);

  QwtScaleWidget *rightAxis = s->axisWidget(QwtPlot::yRight);
  rightAxis->setColorMap(zInterval, new ColorMap());

  s->setAxisScale(QwtPlot::yRight, 0.0, zInterval.maxValue());
}

double RasterData::value(double x, double y) const
{
  if (x >= 0 && x < Statistic::SCREEN_SIZE &&
      y >= 0 && y < Statistic::SCREEN_SIZE)
    return d[qFloor(x)][qFloor(y)] > 0 ? qLn(d[qFloor(x)][qFloor(y)]) : 0.0;
  return 0.0;
}

MouseSpectrogram::MouseSpectrogram(QWidget *parent) :
    QwtPlot(parent),
    operations(NULL),
    currentFrom(0),
    currentTo(TimeLineChart::SECONDS_IN_A_DAY)
{
  data = new RasterData(this);

  spectrogram = new QwtPlotSpectrogram();
  spectrogram->setRenderThreadCount(0);

  spectrogram->setColorMap(new ColorMap());

  spectrogram->setData(data);
  spectrogram->attach(this);

  plotLayout()->setAlignCanvasToScales(true);

  zoomer = new QwtPlotZoomer(canvas());
}

void MouseSpectrogram::clear()
{
  data->clear();
}

void MouseSpectrogram::setOperations(OperationInADay *newOperations)
{
  operations = newOperations;
}

void MouseSpectrogram::setPrograms(QVector<QString> newPrograms)
{
  // Clear
  clear();

  // Set the programs
  programs = newPrograms;

  // Recalculate and flush
  recalculate();
}

void MouseSpectrogram::setTime(int from, int to)
{
  currentFrom = from;
  currentTo = to;
  // Recalculate and flush
  recalculate();
}

void MouseSpectrogram::recalculate()
{
  if (operations == NULL)
    return;
  clear();
  QSet<QString> p;
  QString tmp;
  foreach (tmp, programs)
  {
    p.insert(tmp);
  }

  for (int i = 0;i < operations->mouseOperations.size();++i)
  {
    int second = operations->mouseOperations[i].ms / 1000;
    // Ignore the unselected program
    if (p.contains(operations->mouseOperations[i].program) &&
        second >= currentFrom &&
        second <= currentTo)
    {
      QPoint position = operations->mouseOperations[i].operation.pos;
      data->addOne(position.x(), position.y());
    }
  }
  data->flush();
  replot();
}

MouseSpectrogram::~MouseSpectrogram()
{
  delete zoomer;
  delete spectrogram;
}
