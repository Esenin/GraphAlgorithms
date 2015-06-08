//! @author Ivan Senin

#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

typedef unsigned long long int uint64;

fstream in;
fstream out;
size_t dataSize = 0;
size_t queSize = 0;
vector<uint64> dataArray {};

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

uint64 sumHelper (int const node, int const tl, int const tr, int const left, int const right)
{
  if (left > right)
    return 0;

  if (left == tl && right == tr)
    return dataArray[node];

  int tMiddle = (tl + tr) >> 1;
  return sumHelper (node << 1, tl, tMiddle, left, min(tMiddle, right))
      + sumHelper ((node << 1) + 1, tMiddle + 1, tr, max(left, tMiddle + 1), right);
}

void updateTree (int const node, int const tl, int const tr, int const &index, uint64 const &newValue)
{
  if (tl == tr)
    dataArray[node] = newValue;
  else
    {
      int tMiddle = (tl + tr) / 2;
      if (index > tMiddle)
        updateTree ((node << 1) + 1, tMiddle + 1, tr, index, newValue);
      else
        updateTree (node << 1, tl, tMiddle, index, newValue);

      dataArray[node] = dataArray[node << 1] + dataArray[(node << 1) + 1];
    }
}

void update(int const &index, uint64 const &newValue)
{
  updateTree(1, 0, dataSize - 1, index, newValue);
}

uint64 getSum(int const &leftBound, int const &rightBound)
{
  return sumHelper(1, 0, dataSize - 1, leftBound, rightBound);
}

int main()
{
  initSolution();
  in >> dataSize;
  in >> queSize;
  dataArray.resize(4 * dataSize);

  string taskType("");
  for (size_t i = 0; i < queSize; i++)
    {
      in >> taskType;
      if (taskType == "A")
        {
          int index = 0;
          uint64 x = 0;
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