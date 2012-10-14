#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <qmath.h>
#include <QMap>
#include <QVector>

/**
 * @brief Namespace of some algorithm.
 */
namespace Algorithm
{
  const int MAX_COLOR_VALUE = 255;

  /**
   * @brief Function to get the HSL from RGB.
   * Thanks to http://blog.csdn.net/aniven/article/details/2205851
   *
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   * @param h Hue.
   * @param s Saturation.
   * @param l Lum.
   */
  void RGB2HSL(int r, int g, int b, double& h,double& s,double& l);

  /**
   * @brief Aid function used to do the convertion from HSL to RGB.
   * Thanks to http://blog.csdn.net/aniven/article/details/2205851
   *
   * @param v1 A value.
   * @param v2 Another value.
   * @param vH Hue.
   */
  double Hue2RGB(double v1, double v2, double vH);

  /**
   * @brief Function to get the RGB from HSL.
   * Thanks to http://blog.csdn.net/aniven/article/details/2205851
   *
   * @param h Hue.
   * @param s Saturation.
   * @param l Lum.
   * @param r Red.
   * @param g Green.
   * @param b Blue.
   */
  void HSL2RGB(double h,double s,double l, int& r, int& g, int& b);

  /**
   * @brief Template function to sort key values of a map using the values to compare.
   *
   * @param data The data.
   * @return The sorted keys.
   */
  template<class T>
  QVector<T> sort(const QMap<T, int>& data);

  /**
   * @brief Get the trend of the two counts.
   *
   * Get the trend of the two counts.
   * The return value will between -100 to 100.
   * If a is much bigger than b, the return value will be -100.
   * If b is much bigger than a, the return value will be 100.
   */
  double getTrend(int a, int b);
}

template<class T>
struct KeyIntPair
{
  T key;
  int value;

  KeyIntPair() : key(T()), value(0) {}
  KeyIntPair(T k, int v) : key(k), value(v) {}
};

template<class T>
void swap(T& a, T& b)
{
  T c = a;
  a = b;
  b = c;
}

template<class T>
int partition(KeyIntPair<T> *array, int left, int right)
{
  int index = left;
  KeyIntPair<T> pivot = array[index];
  swap(array[index], array[right]);
  for (int i = left;i < right;++i)
  {
    if (array[i].value > pivot.value)
      swap(array[index++], array[i]);
  }
  swap(array[right], array[index]);
  return index;
}

template<class T>
void qsort(KeyIntPair<T> *array, int left, int right)
{
  if (left >= right)
    return;
  int index = partition(array, left, right);
  qsort(array, left, index - 1);
  qsort(array, index + 1, right);
}

template<class T>
QVector<T> Algorithm::sort(const QMap<T, int>& data)
{
  KeyIntPair<T> *array = new KeyIntPair<T>[data.size()];
  int i = 0;
  for (QMap<QString, int>::ConstIterator itr = data.begin();
       itr != data.end();
       ++itr)
  {
    array[i].key = itr.key();
    array[i].value = itr.value();
    ++i;
  }
  qsort(array, 0, data.size() - 1);
  QVector<T> result;
  for (i = 0;i < data.size();++i)
    result.push_back(array[i].key);
  delete [] array;
  return result;
}

#endif // ALGORITHM_H
