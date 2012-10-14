#include "habitdial.h"

#include <QEvent>

HabitDial::HabitDial(QWidget *parent):
    QDial(parent)
{
  setWrapping(true);
  setRange(-200, 200);
}

QSize	HabitDial::sizeHint() const
{
  return QSize(100, 30);
}

bool HabitDial::event(QEvent *e)
{
  switch (e->type())
  {
  case QEvent::MouseButtonPress:
  case QEvent::MouseButtonRelease:
  case QEvent::MouseMove:
  case QEvent::Wheel:
  case QEvent::KeyPress:
  case QEvent::KeyRelease:
    return true;
  default:
    return QDial::event(e);
  }
}
