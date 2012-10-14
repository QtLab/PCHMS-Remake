#include "keyboardspectrogram.h"

#include <QGridLayout>
#include <QMap>
#include <qmath.h>
#include "keyboard.h"
#include "operationinaday.h"
#include "timelinechart.h"

static const int POSITION_AND_SIZE_OF_THE_KEYBOARD_BUTTON[Keyboard::TOTAL_KEY_NUMBER][4] =
  {
    {3, 20, 2, 2}, {3, 2, 2, 2}, {3, 4, 2, 2}, {3, 6, 2, 2}, {3, 8, 2, 2},
    {3, 10, 2, 2}, {3, 12, 2, 2}, {3, 14, 2, 2}, {3, 16, 2, 2}, {3, 18, 2, 2},
    {7, 26, 2, 4}, {3, 35, 2, 2}, {5, 35, 2, 2}, {3, 33, 2, 2}, {5, 33, 2, 2},
    {5, 31, 2, 2}, {3, 31, 2, 2}, {0, 40, 2, 2}, {0, 44, 2, 2}, {0, 42, 2, 2},
    {3, 38, 2, 2}, {7, 0, 2, 4}, {3, 0, 2, 2}, {3, 22, 2, 2}, {3, 24, 2, 2},
    {3, 26, 2, 4}, {5, 23, 2, 2}, {5, 25, 2, 2}, {5, 27, 2, 3}, {7, 22, 2, 2},
    {7, 24, 2, 2}, {9, 19, 2, 2}, {9, 21, 2, 2}, {9, 23, 2, 2}, {5, 0, 2, 3},
    {9, 33, 2, 2}, {11, 33, 2, 2}, {11, 31, 2, 2}, {11, 35, 2, 2}, {11, 38, 2, 4},
    {9, 38, 2, 2}, {9, 40, 2, 2}, {9, 42, 2, 2}, {7, 38, 2, 2}, {7, 40, 2, 2},
    {7, 42, 2, 2}, {5, 38, 2, 2}, {5, 40, 2, 2}, {5, 42, 2, 2}, {3, 44, 2, 2},
    {5, 44, 4, 2}, {3, 42, 2, 2}, {3, 40, 2, 2}, {9, 44, 4, 2}, {11, 42, 2, 2},
    {0, 0, 2, 2}, {11, 3, 2, 2}, {11, 23, 2, 2}, {11, 25, 2, 2}, {9, 0, 2, 5},
    {9, 25, 2, 5}, {11, 0, 2, 3}, {11, 27, 2, 3}, {11, 5, 2, 3}, {11, 20, 2, 3},
    {7, 4, 2, 2}, {9, 13, 2, 2}, {9, 9, 2, 2}, {7, 8, 2, 2}, {5, 7, 2, 2},
    {7, 10, 2, 2}, {7, 12, 2, 2}, {7, 14, 2, 2}, {5, 17, 2, 2}, {7, 16, 2, 2},
    {7, 18, 2, 2}, {7, 20, 2, 2}, {9, 17, 2, 2}, {9, 15, 2, 2}, {5, 19, 2, 2},
    {5, 21, 2, 2}, {5, 3, 2, 2}, {5, 9, 2, 2}, {7, 6, 2, 2}, {5, 11, 2, 2},
    {5, 15, 2, 2}, {9, 11, 2, 2}, {5, 5, 2, 2}, {9, 7, 2, 2}, {5, 13, 2, 2},
    {9, 5, 2, 2}, {11, 8, 2, 12}, {0, 4, 2, 2}, {0, 6, 2, 2}, {0, 8, 2, 2},
    {0, 10, 2, 2}, {0, 13, 2, 2}, {0, 15, 2, 2}, {0, 17, 2, 2}, {0, 19, 2, 2},
    {0, 22, 2, 2}, {0, 24, 2, 2}, {0, 26, 2, 2}, {0, 28, 2, 2}
  };

KeyboardSpectrogram::KeyboardSpectrogram(QWidget *parent) :
    QWidget(parent),
    currentFrom(0),
    currentTo(TimeLineChart::SECONDS_IN_A_DAY)
{
  QGridLayout *layout = new QGridLayout();
  layout->setSpacing(1);
  buttons = new QPushButton[Keyboard::TOTAL_KEY_NUMBER];
  for (int i = 0;i < Keyboard::TOTAL_KEY_NUMBER;++i)
  {
    buttons[i].setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttons[i].setText(Keyboard::getKeySimpleEnName(Keyboard::keyToString((Keyboard::Key) i)));
    layout->addWidget(buttons + i,
                      POSITION_AND_SIZE_OF_THE_KEYBOARD_BUTTON[i][0],
                      POSITION_AND_SIZE_OF_THE_KEYBOARD_BUTTON[i][1],
                      POSITION_AND_SIZE_OF_THE_KEYBOARD_BUTTON[i][2],
                      POSITION_AND_SIZE_OF_THE_KEYBOARD_BUTTON[i][3]);
  }
  setLayout(layout);
}

void KeyboardSpectrogram::clear()
{
  for (int i = 0;i < Keyboard::TOTAL_KEY_NUMBER;++i)
    buttons[i].setStyleSheet("");
}

void KeyboardSpectrogram::setOperations(OperationInADay *newOperations)
{
  operations = newOperations;
}

void KeyboardSpectrogram::setPrograms(QVector<QString> newPrograms)
{
  // Clear
  clear();

  // Set the programs
  programs = newPrograms;

  // Recalculate and flush
  recalculate();
}

void KeyboardSpectrogram::setTime(int from, int to)
{
  currentFrom = from;
  currentTo = to;
  // Recalculate and flush
  recalculate();
}

void KeyboardSpectrogram::recalculate()
{
  if (operations == NULL)
    return;
  clear();
  QMap<QString, int *> softwareCount;
  int *counts = new int[Keyboard::TOTAL_KEY_NUMBER];
  memset(counts, 0, sizeof(int) * Keyboard::TOTAL_KEY_NUMBER);
  QString tmp;
  foreach (tmp, programs)
  {
    int *count = new int[Keyboard::TOTAL_KEY_NUMBER];
    memset(count, 0, sizeof(int) * Keyboard::TOTAL_KEY_NUMBER);
    softwareCount.insert(tmp, count);
  }

  int max = 1;

  // Go through the softwares and calculate the counts.
  for (int i = 0;i < operations->keyOperations.size();++i)
  {
    int second = operations->keyOperations[i].ms / 1000;
    Keyboard::Key key = (Keyboard::Key) operations->keyOperations[i].operation.key;
    if (second >= currentFrom &&
        second <= currentTo)
    {
      int *count = softwareCount.value(operations->keyOperations[i].program, NULL);
      if (count != NULL && key >= 0 && key < Keyboard::TOTAL_KEY_NUMBER)
      {
        ++count[key];
        ++counts[key];
        max = qMax(max, counts[key]);
      }
    }
  }

  // Get the tooltips of each key
  QString *tips = new QString[Keyboard::TOTAL_KEY_NUMBER];
  for (QMap<QString, int*>::Iterator itr = softwareCount.begin();
       itr != softwareCount.end();
       ++itr)
  {
    QString program = itr.key();
    int *count = itr.value();
    for (int i = 0;i < Keyboard::TOTAL_KEY_NUMBER;++i)
      if (count[i] != 0)
        tips[i].append(tr("%1:%2\n").arg(program).arg(count[i]));
  }

  // Set the buttons
  double dMax = qLn(max);
  for (int i = 0;i < Keyboard::TOTAL_KEY_NUMBER;++i)
  {
    buttons[i].setObjectName(tr("button%1").arg(i));
    buttons[i].setStyleSheet(tr("QPushButton#button%1{background-color:rgba(255,0,0,%2)}").
                             arg(i).arg(counts[i] >= 1 ? 55  + qLn(counts[i]) * 185 / dMax : 25));
    buttons[i].setToolTip(tips[i]);
  }


  // Clear
  for (QMap<QString, int*>::Iterator itr = softwareCount.begin();
       itr != softwareCount.end();
       ++itr)
  {
    delete [] itr.value();
  }
  delete [] counts;
  delete [] tips;
}

KeyboardSpectrogram::~KeyboardSpectrogram()
{
  delete [] buttons;
}
