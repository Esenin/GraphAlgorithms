//! @author Ivan Senin
//! @brief Fenwick tree

#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

typedef long long int int64;

fstream in;
fstream out;
size_t dataSize = 0;
size_t queSize = 0;
vector<int64> dataArray {};

void initSolution()
{
  ios_base::sync_with_stdio(false);
  string const fileName("sum");

  in.open(fileName + ".in", ios_base::in);
  out.open(fileName + ".out", ios_base::out | ios_base::trunc);
}

void postActions()
{
  in.close();
  out.close();
}



inline int fPrev(int const x)
{
  return (x & (x + 1)) - 1;
}

inline int fNext(int const x)
{
  return x | (x + 1);
}

int64 sumHelper(int rightBound)
{
  int64 result = 0;
  while (rightBound >= 0)
    {
      result += dataArray[rightBound];
      rightBound = fPrev(rightBound);
    }
  return result;
}


void add(size_t index, int64 newValue)
{
  while (index < dataSize)
    {
      dataArray[index] += newValue;
      index = fNext(index);
    }
}

void update(size_t const &index, int64 const &newValue)
{
  add(index, newValue - dataArray[dataSize + index]);
  dataArray[dataSize + index] = newValue;
}

int64 getSum(size_t const &leftBound, size_t const &rightBound)
{
  return sumHelper(rightBound) - sumHelper(leftBound - 1);
}

int main()
{
  initSolution();
  in >> dataSize;
  in >> queSize;
  dataArray.resize(2 * dataSize);

  string taskType("");
  for (size_t i = 0; i < queSize; i++)
    {
      in >> taskType;
      if (taskType == "A")
        {
          int index = 0;
          int64 x = 0;
          in >> index >> x;
          update(index - 1, x);
        }
      else
        {
          int l = 0, r = 0;
          in >> l >> r;
          out << getSum(l - 1, r - 1) << "\n";
        }
    }


  postActions();
  return 0;
}