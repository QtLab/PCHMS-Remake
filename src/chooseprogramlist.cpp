#include "chooseprogramlist.h"

#include "colormanagement.h"

const int ChooseProgramList::MAX_AUTO_CHECK = 5;

ChooseProgramList::ChooseProgramList(QWidget *parent) :
    QWidget(parent)
{
  layout = new QVBoxLayout();
  this->setLayout(layout);
}

void ChooseProgramList::setPrograms(const QVector<QString>& programs)
{
  // Clear old check boxes
  QCheckBox *checkBox;
  foreach (checkBox, checkBoxes)
    delete checkBox;
  checkBoxes.clear();

  // Create new check boxes
  checkBoxes.reserve(programs.size());
  for(int i = 0;i < programs.size();++i)
  {
    checkBox = new QCheckBox(programs[i]);
    checkBox->setChecked(i < MAX_AUTO_CHECK);
    connect(checkBox,
            SIGNAL(toggled(bool)),
            SLOT(checkChanged(bool)));
    layout->addWidget(checkBox, 0, Qt::AlignLeft);
    checkBoxes.push_back(checkBox);
  }
  resetColor();
}

QVector<QString> ChooseProgramList::getSelectedPrograms() const
{
  QVector<QString> result;
  QCheckBox *checkBox;
  foreach (checkBox, checkBoxes)
  {
    if (checkBox->isChecked())
      result.push_back(checkBox->text());
  }
  return result;
}

void ChooseProgramList::resetColor()
{
  // Get the count of all checked programs
  int count1 = 0;
  QCheckBox *checkBox;
  foreach (checkBox, checkBoxes)
  {
    if (checkBox->isChecked())
      ++count1;
  }
  // Others programs
  ++count1;

  // Set the color of the check boxes
  int count2 = 0;
  foreach (checkBox, checkBoxes)
  {
    QColor color = ColorManagement::getChartColor((checkBox->isChecked() ?
                                                   count2 :
                                                   count1 - 1) * 1.0 / count1);
    checkBox->setStyleSheet(tr("color: rgb(%1, %2, %3);").
                            arg(color.red()).
                            arg(color.green()).
                            arg(color.blue()));
    if (checkBox->isChecked())
      ++count2;
  }
}

void ChooseProgramList::checkChanged(bool checked)
{
  resetColor();
  emit selectionChanged();
}

ChooseProgramList::~ChooseProgramList()
{
  QCheckBox *checkBox;
  foreach (checkBox, checkBoxes)
    delete checkBox;
  delete layout;
}
