#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include "aboutauthordialog.h"
#include "aboutprojectdialog.h"
#include "algorithm.h"
#include "inadaywidget.h"
#include "operationinaday.h"
#include "programwidget.h"
#include "recorder.h"
#include "suggestion.h"

#define MOUSE_NAME QString::fromUtf8("鼠标")
#define KEYBOARD_NAME QString::fromUtf8("键盘")

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->windowMenu->addAction(ui->activeTimeDock->toggleViewAction());
  ui->windowMenu->addAction(ui->deviceHabitDock->toggleViewAction());
  ui->windowMenu->addAction(ui->deviceBusyDock->toggleViewAction());
  ui->windowMenu->addAction(ui->programHabitDock->toggleViewAction());

  splitDockWidget(ui->deviceHabitDock, ui->deviceBusyDock, Qt::Vertical);
  statistics = Recorder::readAllStatistic();
  currentLevel = Statistic::Month;
  currentDate = statistics->getEndDate();
  recalculate();

  connect(ui->softwareTime,
          SIGNAL(selected(QString)),
          SLOT(showProgram(QString)));
  connect(ui->softwareOperation,
          SIGNAL(selected(QString)),
          SLOT(showProgram(QString)));
  connect(ui->globalChart,
          SIGNAL(zoomIn(int)),
          SLOT(zoomIn(int)));
  connect(ui->globalChart,
          SIGNAL(zoomOut()),
          SLOT(zoomOut()));

  connect(ui->exitAction,
          SIGNAL(triggered()),
          qApp,
          SLOT(quit()));
  connect(ui->aboutQtAction,
          SIGNAL(triggered()),
          qApp,
          SLOT(aboutQt()));
  connect(ui->aboutAuthorAction,
          SIGNAL(triggered()),
          SLOT(aboutAuthor()));
  connect(ui->aboutProjectAction,
          SIGNAL(triggered()),
          SLOT(aboutProject()));
}

void MainWindow::setVisible(bool visible)
{
  QWidget::setVisible(visible);
  if (!visible)
    qApp->quit();
}

void MainWindow::childDestroyed(QObject *obj)
{
  children.remove(obj);
}

void MainWindow::showDetail(QDate date)
{
  OperationInADay *operations = Recorder::readOperationsInADay(date);
  InADayWidget *w = new InADayWidget(operations);
  w->setWindowTitle(date.toString("yyyyMMdd"));
  children.insert(w);
  connect(w,
          SIGNAL(destroyed(QObject*)),
          SLOT(childDestroyed(QObject*)));
  w->show();
}

void MainWindow::showProgram(QString program)
{
  Statistic *statistic = getCurrentStatistic();
  ProgramWidget *w = new ProgramWidget(statistic == NULL ?
                                       NULL :
                                       statistic->getProgramStatistic().value(program, NULL));
  w->setWindowTitle(getDateString(statistic) + ":" + program);
  children.insert(w);
  connect(w,
          SIGNAL(destroyed(QObject*)),
          SLOT(childDestroyed(QObject*)));
  w->show();
}

Statistic *MainWindow::getCurrentStatistic()
{
  Statistic *currentStatistic = statistics;
  while (currentStatistic->getLevel() < currentLevel)
  {
    Statistic *tmp = NULL;
    foreach (tmp, currentStatistic->getSubStatistics())
    {
      // Find the sub one.
      if (((tmp->getLevel() == Statistic::Year) &&
           (currentDate.year() == tmp->getStartDate().year())) ||
          ((tmp->getLevel() == Statistic::Month) &&
           (currentDate.year() == tmp->getStartDate().year()) &&
           (currentDate.month() == tmp->getStartDate().month())))
      {
        currentStatistic = tmp;
        break;
      }
    }
    // Should have a sub one, but there's no sub one.
    if (tmp == NULL)
      return NULL;
  }
  return currentStatistic;
}

QString MainWindow::getDateString(Statistic *statistic)
{
  if (statistic == NULL)
    return QString();
  switch (statistic->getLevel())
  {
  case Statistic::All:
    return QString::fromUtf8("All");
  case Statistic::Year:
    return statistic->getStartDate().toString("yyyy");
  case Statistic::Month:
    return statistic->getStartDate().toString("yyyyMM");
  default:
    // Not supposed to be reached
    return QString();
  }
}

void MainWindow::recalculate()
{
  Statistic *currentStatistic = getCurrentStatistic();
  if (currentStatistic == NULL)
    return;

  QVector<QString> suggestions;
  QVector<QString> details;

  // Clear the summary
  {
    ui->suggestionListView->clear();
  }

  // Active period
  {
    ui->activePeriod->setValues(currentStatistic->getWholeStatistic()->activeTime);
    Suggestion::activePeriodSuggestion(currentStatistic->getWholeStatistic()->activeTime,
                                       suggestions,
                                       details);
    ui->activePeriod->setToolTip(Suggestion::getTooltip(suggestions));
    ui->suggestionListView->append(suggestions, details);
  }

  // Program
  {
    // Get the sorted programs
    // Using the active time to sort
    QMap<QString, int> p;
    const QMap<QString, Statistic::StatisticUnit *>& programStatistics = currentStatistic->getProgramStatistic();
    for (QMap<QString, Statistic::StatisticUnit *>::ConstIterator itr = programStatistics.begin();
         itr != programStatistics.end();
         ++itr)
    {
      p.insert(itr.key(), itr.value()->globalActiveTime);
    }
    QVector<QString> programs = Algorithm::sort(p);

    // Get the operation count and time of the programs
    QVector<int> programOperations;
    QVector<int> programTimes;
    foreach (QString program, programs)
    {
      // Get the operation count
      const Statistic::StatisticUnit *programStatistic = programStatistics.value(program);
      const QMap<Keyboard::Key, int>& keyTimes = programStatistic->keyTimes;
      int count = 0;
      for (QMap<Keyboard::Key, int>::ConstIterator itr = keyTimes.begin();
           itr != keyTimes.end();
           ++itr)
        count += itr.value();
      programOperations.push_back(count);

      // Get the time
      programTimes.push_back(programStatistic->globalActiveTime);
    }

    // Set the two pie views
    ui->softwareOperation->setValues(programs, programOperations);
    ui->softwareTime->setValues(programs, programTimes);
  }

  // Device
  {
    // Get the operation count of the devices
    int keyCount = 0;
    int buttonCount = 0;
    const Statistic::StatisticUnit *unit = currentStatistic->getWholeStatistic();
    for (QMap<Keyboard::Key, int>::ConstIterator itr = unit->keyTimes.begin();
         itr != unit->keyTimes.end();
         ++itr)
      keyCount += itr.value();
    for (QMap<Mouse::Button, int>::ConstIterator itr = unit->buttonTimes.begin();
         itr != unit->buttonTimes.end();
         ++itr)
      buttonCount += itr.value();

    // Set the value and tooltip and summary
    ui->deviceOperation->setValue(Algorithm::getTrend(keyCount, buttonCount));
    Suggestion::getDeviceTrend(KEYBOARD_NAME,
                               MOUSE_NAME,
                               keyCount,
                               buttonCount,
                               suggestions,
                               details);
    ui->deviceOperation->setToolTip(Suggestion::getTooltip(suggestions));
    ui->suggestionListView->append(suggestions, details);

    ui->deviceTime->setValue(Algorithm::getTrend(unit->keyboardActiveTime,
                                                 unit->mouseActiveTime));
    Suggestion::getDeviceTrend(KEYBOARD_NAME,
                               MOUSE_NAME,
                               unit->keyboardActiveTime,
                               unit->mouseActiveTime,
                               suggestions,
                               details);
    ui->deviceTime->setToolTip(Suggestion::getTooltip(suggestions));
    ui->suggestionListView->append(suggestions, details);

    QString suggestion;
    QString detail;

    ui->keyboardBusy->setValue(Algorithm::getTrend(keyCount * 5,
                                                   unit->keyboardActiveTime * 2));
    Suggestion::getBusyTrend(keyCount,
                             unit->keyboardActiveTime,
                             suggestion,
                             detail);
    ui->keyboardBusy->setToolTip(suggestion);
    ui->suggestionListView->append(tr("%1 %2").arg(KEYBOARD_NAME).arg(suggestion),
                                        detail);

    ui->mouseBusy->setValue(Algorithm::getTrend(buttonCount * 5,
                                                unit->mouseActiveTime * 2));
    Suggestion::getBusyTrend(buttonCount,
                             unit->mouseActiveTime,
                             suggestion,
                             detail);
    ui->mouseBusy->setToolTip(suggestion);
    ui->suggestionListView->append(tr("%1 %2").arg(MOUSE_NAME).arg(suggestion),
                                        detail);
  }

  ui->globalChart->recalculate(currentStatistic);
}

void MainWindow::zoomIn(int x)
{
  switch (currentLevel)
  {
  case Statistic::All:
    {
      currentDate = Statistic::BASE_DATE.addYears(x);
      currentLevel = Statistic::Year;
      recalculate();
      break;
    }
  case Statistic::Year:
    {
      currentDate = Statistic::BASE_DATE.addYears(x / 12);
      currentDate = currentDate.addMonths(x % 12);
      currentLevel = Statistic::Month;
      recalculate();
      break;
    }
  case Statistic::Month:
    {
      QDate date = Statistic::BASE_DATE.addDays(x);
      showDetail(date);
      break;
    }
  default:
    break;
  }
}

void MainWindow::zoomOut()
{
  switch (currentLevel)
  {
  case Statistic::Year:
  case Statistic::Month:
    {
      currentLevel = (Statistic::StatisticLevel) (currentLevel - 1);
      recalculate();
    }
  default:
    break;
  }
}

void MainWindow::aboutAuthor()
{
  AboutAuthorDialog d(this);
  d.exec();
}

void MainWindow::aboutProject()
{
  AboutProjectDialog d(this);
  d.exec();
}

MainWindow::~MainWindow()
{
  // Delete children
  QObject *obj;
  foreach (obj, children)
  {
    disconnect(obj,
               SIGNAL(destroyed(QObject*)),
               this,
               SLOT(childDestroyed(QObject*)));
    delete obj;
  }
  delete ui;
  delete statistics;
}
