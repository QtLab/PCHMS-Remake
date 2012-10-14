#include "operationlistview.h"

#include "operationinaday.h"
#include "timelinechart.h"

OperationListView::OperationListView(QWidget *parent) :
    QTableView(parent),
    operations(NULL)
{
  model = new QStandardItemModel(0, 4);
  QStringList labels;
  labels.push_back(QString::fromUtf8("时间"));
  labels.push_back(QString::fromUtf8("程序"));
  labels.push_back(QString::fromUtf8("设备"));
  labels.push_back(QString::fromUtf8("操作"));
  model->setHorizontalHeaderLabels(labels);
  setModel(model);
  setColumnWidth(3, 225);
}

void OperationListView::setOperations(OperationInADay *newOperations)
{
  operations = newOperations;
}

void OperationListView::clear()
{
  model->removeRows(0, model->rowCount());
}

void OperationListView::recalculate()
{
  if (operations == NULL)
    return;
  clear();

  // Go through the operations
  int currentKeyIndex = 0;
  int currentMouseIndex = 0;

  // Prepare the space
  model->setRowCount(operations->keyOperations.size() + operations->mouseOperations.size());

  int row = 0;
  while (currentKeyIndex < operations->keyOperations.size() ||
         currentMouseIndex < operations->mouseOperations.size())
  {
    bool key = currentMouseIndex >= operations->mouseOperations.size() ||
               (currentKeyIndex < operations->keyOperations.size() &&
                operations->keyOperations[currentKeyIndex].id <
                operations->mouseOperations[currentMouseIndex].id);
    QString program;
    QString device;
    QString operationStr;
    int ms;

    // Get the information
    if (key)
    {
      program = operations->keyOperations[currentKeyIndex].program;
      ms = operations->keyOperations[currentKeyIndex].ms;
      Keyboard::Key key = operations->keyOperations[currentKeyIndex].operation.key;
      Keyboard::Operation op = operations->keyOperations[currentKeyIndex].operation.operation;
      operationStr = ((op == Keyboard::Down) ?
                      QString::fromUtf8("按下") :
                      QString::fromUtf8("放开")) +
                     Keyboard::getKeyFullZhName(Keyboard::keyToString(key));
      device = QString::fromUtf8("键盘");
    }
    else
    {
      program = operations->mouseOperations[currentMouseIndex].program;
      ms = operations->mouseOperations[currentMouseIndex].ms;
      Mouse::Button button = operations->mouseOperations[currentMouseIndex].operation.button;
      Mouse::Operation op = operations->mouseOperations[currentMouseIndex].operation.operation;
      operationStr = ((op == Mouse::Down) ?
                      QString::fromUtf8("按下") :
                      QString::fromUtf8("放开")) +
                     Mouse::buttonToString(button) +
                     tr("(%1, %2)").
                     arg(operations->mouseOperations[currentMouseIndex].operation.pos.x()).
                     arg(operations->mouseOperations[currentMouseIndex].operation.pos.y());
      device = QString::fromUtf8("鼠标");
    }

    // Add a row
    {
      // Calculate the string for the time, the format should be 12:34:56:789
      QString timeStr;
      {
        timeStr += tr("%1:").arg(ms / 3600000, 2, 10, QChar('0'));
        ms = ms % 3600000;
        timeStr += tr("%1:").arg(ms / 60000, 2, 10, QChar('0'));
        ms = ms % 60000;
        timeStr += tr("%1:%2").
                   arg(ms / 1000, 2, 10, QChar('0')).
                   arg(ms % 1000, 3, 10, QChar('0'));
      }
      model->setData(model->index(row, 0), timeStr);
      model->setData(model->index(row, 1), program);
      model->setData(model->index(row, 2), device);
      model->setData(model->index(row, 3), operationStr);
      ++row;
    }

    // Increase the index
    if (key)
      ++currentKeyIndex;
    else
      ++currentMouseIndex;
  }
}

void OperationListView::setVisible(bool visible)
{
  QWidget::setVisible(visible);
  if (!visible)
    delete this;
}

OperationListView::~OperationListView()
{
  delete model;
}
