#ifndef KEYBOARDSPECTROGRAM_H
#define KEYBOARDSPECTROGRAM_H

#include <QWidget>

#include <QPushButton>
#include <QVector>

class OperationInADay;

/**
 * @brief Class of spectrogram to show hot keys on keyboard.
 */
class KeyboardSpectrogram : public QWidget
{
  Q_OBJECT
public:
  KeyboardSpectrogram(QWidget *parent=NULL);
  ~KeyboardSpectrogram();

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

  /**
   * The buttons.
   */
  QPushButton *buttons;
};

#endif // KEYBOARDSPECTROGRAM_H
