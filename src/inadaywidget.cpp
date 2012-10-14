#include "inadaywidget.h"
#include "ui_inadaywidget.h"

#include "algorithm.h"
#include "operationinaday.h"
#include "operationlistview.h"

// TODO: replay

InADayWidget::InADayWidget(OperationInADay *o,
                           QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InADayWidget),
    operations(o)
{
  ui->setupUi(this);

  QVector<QString> p = Algorithm::sort(operations->counts);
  QVector<QString> programs;
  for (int i = 0;i < ChooseProgramList::MAX_AUTO_CHECK && i < p.size();++i)
    programs.push_back(p[i]);

  ui->inADayOperationChart->setPrograms(programs);
  ui->inADayOperationChart->setOperations(operations);
  ui->inADayTimeChart->setPrograms(programs);
  ui->inADayTimeChart->setOperations(operations);
  ui->mouseSpectrogram->setOperations(operations);
  ui->mouseSpectrogram->setPrograms(programs);
  ui->keyboardSpectrogram->setOperations(operations);
  ui->keyboardSpectrogram->setPrograms(programs);
  ui->chooseProgramList->setPrograms(p);
  connect(ui->chooseProgramList,
          SIGNAL(selectionChanged()),
          SLOT(selectionChanged()));
  connect(ui->inADayOperationChart,
          SIGNAL(timeChanged(int,int)),
          ui->inADayTimeChart,
          SLOT(zoomIn(int,int)));
  connect(ui->inADayTimeChart,
          SIGNAL(timeChanged(int,int)),
          ui->inADayOperationChart,
          SLOT(zoomIn(int,int)));
  connect(ui->inADayOperationChart,
          SIGNAL(timeChanged(int,int)),
          ui->mouseSpectrogram,
          SLOT(setTime(int,int)));
  connect(ui->inADayOperationChart,
          SIGNAL(timeChanged(int,int)),
          ui->keyboardSpectrogram,
          SLOT(setTime(int,int)));
  connect(ui->operationListAction,
          SIGNAL(triggered()),
          SLOT(showOperationList()));
}

void InADayWidget::selectionChanged()
{
  QVector<QString> programs = ui->chooseProgramList->getSelectedPrograms();
  ui->inADayOperationChart->setPrograms(programs);
  ui->inADayTimeChart->setPrograms(programs);
  ui->mouseSpectrogram->setPrograms(programs);
  ui->keyboardSpectrogram->setPrograms(programs);
}

void InADayWidget::showOperationList()
{
  // Create a new widget to show the list.
  // The list will cost much memory, so I didn't put it in this widget.
  OperationListView *view = new OperationListView();
  view->setOperations(operations);
  view->recalculate();
  view->setWindowTitle(windowTitle());
  view->resize(600, 500);
  view->show();
}

void InADayWidget::setVisible(bool visible)
{
  QWidget::setVisible(visible);
  if (!visible)
    delete this;
}

InADayWidget::~InADayWidget()
{
  delete operations;
  delete ui;
}
