//! @file
//! @author Ivan Senin
//! @brief Problem ZP. /. hungarian_algo
//!

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cstring>
#include <limits>
#include <functional>

#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <array>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

/**************************************************************************************************
 *                             Macros
 * ***********************************************************************************************/

#ifdef HOMERUN
  #include <chrono>
#endif

#ifdef WIN
  #define LLD "%I64d"
#else
  #define LLD "%lld"
#endif

#define FORR(size_) \
  size_t const _lineSize_ = size_; \
  size_t i = startWith(_lineSize_); \
  for (uint _i_ = 0; _i_ < _lineSize_; ++_i_, i = (i + 1) % _lineSize_)

#define DEBUG(...) { \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); }

/**************************************************************************************************
 *                             Typedefs & Constants
 * ***********************************************************************************************/

using namespace std;

typedef unsigned long long int uint64;
typedef long long int int64;
typedef unsigned int uint;


template<class T>
using wector = vector<vector<T>>;

constexpr int64 Inf64 = numeric_limits<int64>::max();
constexpr int Inf = numeric_limits<int>::max();
constexpr uint uInf = numeric_limits<uint>::max();
constexpr uint64 uInf64 = numeric_limits<uint64>::max();

/**************************************************************************************************
 *                             Common Helpers
 * ***********************************************************************************************/

void ioInit()
{
#ifndef HOMERUN
  const char *inName = "assignment.in";
  const char *outName = "assignment.out";
#else
  const char *inName = "input";
  const char *outName = "output";
#endif

  freopen(inName, "r", stdin);
  freopen(outName, "w", stdout);
}

void ioClose()
{
  fclose(stdin);
  fclose(stdout);
}

//!
//! @brief randomm
//! @author Ivan Senin
//! @param x unsigned int as seed
//! @return unsigned int less that 100k + 3
size_t randomm(size_t x)
{
  static size_t prevRand = 0;
  x = x << (x & 15);
  x = x * prevRand;
  x = x | (~x&(x + 1));
  prevRand = x % 100003; //prime
  return prevRand;
}

size_t startWith(size_t size)
{
  if (!size)
    return 0;
  else if (size == 1)
    return 0;
  return randomm(size) % size;
}

/**************************************************************************************************
 *                             Solution goes here
 * ***********************************************************************************************/
int const nMax = 300 + 1;

typedef array<int, nMax> cline;

array<cline, nMax> cost;
array<int, nMax> minForCols, rLoad, cLoad, pairMatches, prevCol;
array<bool, nMax> used;
int n;

//! @attention 1-indexed matrix -- cost
void hungarian_algo()
{
  for (int mRow = 1; mRow <= n; mRow++) // include next matrix row
    {
      pairMatches[0] = mRow;
      int markedCol = 0;
      fill(minForCols.begin(), minForCols.end(), Inf);
      fill(used.begin(), used.end(), false);
      do
        {
          used[markedCol] = true;
          int markedRow = pairMatches[markedCol];
          int delta = Inf;
          int colWithMin = 0;

          for (int j = 1; j <= n; j++) // update minimums, delta
            {
              if (used[j])
                continue;

              int cur = cost[markedRow][j] - rLoad[markedRow] - cLoad[j];
              if (cur < minForCols[j])
                {
                  minForCols[j] = cur;
                  prevCol[j] = markedCol;
                }
              if (minForCols[j] < delta)
                {
                  delta = minForCols[j];
                  colWithMin = j;
                }

            }
//          DEBUG("d = %d", delta);

          if (delta != 0)
            {
              for (int j = 0; j <= n; j++) // recalc charges
                if (used[j])
                  {
                    rLoad[pairMatches[j]] += delta;
                    cLoad[j] -= delta;
                  }
                else
                  minForCols[j] -= delta;
            }

//          DEBUG("min col = %d", colWithMin);
          markedCol = colWithMin;
        } while (pairMatches[markedCol] != 0); // free col was found

      do
        {
          int p = prevCol[markedCol];
          pairMatches[markedCol] = pairMatches[p];
          markedCol = p;
        }
      while (markedCol != 0);
    }
}



int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  scanf("%d", &n);
  for (int i = 1; i <= n; i++)
    for(int j = 1; j <= n; j++)
      {
        int co;
        scanf("%d", &co);
        cost[i][j] = co;
      }

  hungarian_algo();

  printf("%d\n", -cLoad[0]);
  for (int i = 1; i <= n; i++)
    printf("%d %d\n", pairMatches[i], i);


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}