#include "programwidget.h"
#include "ui_programwidget.h"

#include "algorithm.h"

ProgramWidget::ProgramWidget(Statistic::StatisticUnit *s,
                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramWidget),
    statistic(s)
{
  ui->setupUi(this);
  model = new QStandardItemModel(0, 2);
  QStringList labels;
  labels.push_back(QString::fromUtf8("组合键"));
  labels.push_back(QString::fromUtf8("次数"));
  model->setHorizontalHeaderLabels(labels);
  ui->comboKeyTable->setModel(model);
  ui->comboKeyTable->setColumnWidth(0, 225);
  recalculate();
}

void ProgramWidget::recalculate()
{
  if (statistic == NULL)
    return;

  // Active period
  {
    ui->activePeriod->setValues(statistic->activeTime);
  }

  // Device
  {
    int keyCount = 0;
    int buttonCount = 0;
    for (QMap<Keyboard::Key, int>::ConstIterator itr = statistic->keyTimes.begin();
         itr != statistic->keyTimes.end();
         ++itr)
      keyCount += itr.value();
    for (QMap<Mouse::Button, int>::ConstIterator itr = statistic->buttonTimes.begin();
         itr != statistic->buttonTimes.end();
         ++itr)
      buttonCount += itr.value();
    ui->deviceOperation->setValue(Algorithm::getTrend(keyCount, buttonCount));
    ui->deviceTime->setValue(Algorithm::getTrend(statistic->keyboardActiveTime,
                                                 statistic->mouseActiveTime));
    ui->keyboardBusy->setValue(Algorithm::getTrend(keyCount * 5,
                                                   statistic->keyboardActiveTime * 2));
    ui->mouseBusy->setValue(Algorithm::getTrend(buttonCount * 5,
                                                statistic->mouseActiveTime * 2));
  }

  // Combo key
  {
    int row = 0;
    for (QMap<Keyboard::ComboKey, int>::ConstIterator itr = statistic->comboKeyTimes.begin();
         itr != statistic->comboKeyTimes.end();
         ++itr)
    {
      model->insertRows(row, 1, QModelIndex());
      model->setData(model->index(row, 0), Keyboard::comboKeyToString(itr.key()));
      model->setData(model->index(row, 1), itr.value());
      ++row;
    }
    // Sort automatically
    ui->comboKeyTable->sortByColumn(1, Qt::DescendingOrder);
  }
}

void ProgramWidget::setVisible(bool visible)
{
  QWidget::setVisible(visible);
  if (!visible)
    delete this;
}

ProgramWidget::~ProgramWidget()
{
  delete model;
  delete ui;
}
