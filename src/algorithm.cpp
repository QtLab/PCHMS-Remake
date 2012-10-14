#include "algorithm.h"

void Algorithm::RGB2HSL(int r, int g, int b, double& h,double& s,double& l)
{
  double dr, dg, db, max, min, del_R, del_G, del_B, del_Max;
  dr = (double)r / MAX_COLOR_VALUE;
  dg = (double)g / MAX_COLOR_VALUE;
  db = (double)b / MAX_COLOR_VALUE;

  min = qMin(dr, qMin(dg, db));
  max = qMax(dr, qMax(dg, db));
  del_Max = max - min;

  l = (max + min) / 2.0;

  if (del_Max == 0)
  {
    h = 0;
    s = 0;
  }
  else
  {
    if (l < 0.5)
      s = del_Max / (max + min);
    else
      s = del_Max / (2 - max - min);

    del_R = (((max - dr) / 6.0) + (del_Max / 2.0)) / del_Max;
    del_G = (((max - dg) / 6.0) + (del_Max / 2.0)) / del_Max;
    del_B = (((max - db) / 6.0) + (del_Max / 2.0)) / del_Max;

    if (dr == max)
      h = del_B - del_G;
    else if (dg == max)
      h = (1.0 / 3.0) + del_R - del_B;
    else if (db == max)
      h = (2.0 / 3.0) + del_G - del_R;

    if (h < 0)
      h += 1;
    if (h > 1)
      h -= 1;
  }
}

double Algorithm::Hue2RGB(double v1, double v2, double vH)
{
  if (vH < 0)
    vH += 1;
  if (vH > 1)
    vH -= 1;
  if (6.0 * vH < 1)
    return v1 + (v2 - v1) * 6.0 * vH;
  if (2.0 * vH < 1)
    return v2;
  if (3.0 * vH < 2)
    return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
  return (v1);
}

void Algorithm::HSL2RGB(double h,double s,double l, int& r, int& g, int& b)
{
  double var_1, var_2;
  if (s == 0)
  {
      r = l * MAX_COLOR_VALUE;
      g = l * MAX_COLOR_VALUE;
      b = l * MAX_COLOR_VALUE;
  }
  else
  {
    if (l < 0.5)
      var_2 = l * (1 + s);
    else
      var_2 = (l + s) - (s * l);

    var_1 = 2.0 * l - var_2;

    r = MAX_COLOR_VALUE * Hue2RGB(var_1, var_2, h + (1.0 / 3.0));
    g = MAX_COLOR_VALUE * Hue2RGB(var_1, var_2, h);
    b = MAX_COLOR_VALUE * Hue2RGB(var_1, var_2, h - (1.0 / 3.0));
  }
}

double Algorithm::getTrend(int a, int b)
{
  double result = (a != 0 && b != 0) ? qLn(1.0 * b / a) :
                  (a != 0 && b == 0 ? -100 :
                   (a == 0 && b != 0 ? 100 : 0));
  result = qBound(-2.0, result, 2.0);
  return result * 50;
}
