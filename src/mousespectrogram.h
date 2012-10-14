#ifndef MOUSESPECTROGRAM_H
#define MOUSESPECTROGRAM_H

#include <qwt_plot.h>

#include <QVector>
#include <qwt_plot_spectrogram.h>
#include <qwt_plot_zoomer.h>

class OperationInADay;
class RasterData;

/**
 * @brief Class of spectrogram to show hot points on mouse.
 */
class MouseSpectrogram : public QwtPlot
{
  Q_OBJECT
public:
  MouseSpectrogram(QWidget *parent=NULL);
  ~MouseSpectrogram();

  /**
   * @brief Set the raw data of the operations.
   */
  void setOperations(OperationInADay *newOperations);

  /**
   * @brief Clear the spectrogram.
   */
  void clear();

  /**
   * @brief Recalculate the spectrogram and flush.
   */
  void recalculate();

public slots:
  /**
   * @brief Set the programs.
   */
  void setPrograms(QVector<QString> newPrograms);

  /**
   * @brief A slot to set time.
   */
  void setTime(int from, int to);

private:
  /**
   * The operations.
   */
  OperationInADay *operations;

  /**
   * The data of the spectrogram.
   */
  RasterData *data;

  /**
   * The spectrogram.
   */
  QwtPlotSpectrogram *spectrogram;

  /**
   * The zoomer.
   */
  QwtPlotZoomer *zoomer;

  /**
   * From.
   */
  int currentFrom;

  /**
   * To.
   */
  int currentTo;

  /**
   * The programs to watch.
   */
  QVector<QString> programs;
};

#endif // MOUSESPECTROGRAM_H
