#include "topwidget.h"

TopWidget::TopWidget(QWidget *parent) :
    QWidget(parent)
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup | Qt::Tool);
}

void TopWidget::setOnTop(bool top)
{
  if (top)
  {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Popup | Qt::Tool);
    show();
  }
  else
  {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    show();
  }
}
