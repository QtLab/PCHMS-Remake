#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDate>
#include <QSet>
#include "statistic.h"

namespace Ui {
  class MainWindow;
}

/**
 * @brief Class of the main window.
 *
 * Class of the main window.
 * Will read the statistics when it is created.
 * Will destroy itself when hidden.
 * Will recycle the windows created by it automatically when it is destroyed.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  virtual void setVisible(bool visible);

private:
  Ui::MainWindow *ui;

  /**
   * The statistics.
   */
  Statistic *statistics;

  /**
   * The windows created by it.
   */
  QSet<QObject *> children;

  /**
   * Current date viewing.
   */
  QDate currentDate;

  /**
   * Current level of the statistic viewing.
   */
  Statistic::StatisticLevel currentLevel;

  /**
   * @brief Recalculate the charts for current statistic viewing.
   */
  void recalculate();

  /**
   * @brief Get the current statistic according to currentDate and currentLevel.
   */
  Statistic *getCurrentStatistic();

  /**
   * @brief Get the date string of the statistic.
   */
  static QString getDateString(Statistic *statistic);

private slots:
  /**
   * @brief A slot to be called when the window created by it is destroyed.
   */
  void childDestroyed(QObject *obj);

  /**
   * @brief A slot to show the detail operation in a day.
   */
  void showDetail(QDate date);

  /**
   * @brief A slot to show the detail statistic of a program.
   */
  void showProgram(QString program);

  /**
   * @brief A slot to zoom in.
   */
  void zoomIn(int x);

  /**
   * @brief A slot to zoom out.
   */
  void zoomOut();

  /**
   * @brief A slot to show things about author.
   */
  void aboutAuthor();

  /**
   * @brief A slot to show things about project.
   */
  void aboutProject();
};

#endif // MAINWINDOW_H
