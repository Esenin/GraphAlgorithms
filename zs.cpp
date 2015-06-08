//! @file
//! @author Ivan Senin
//! @brief Problem ZS. Substring comparation, suffix array
//!

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cstring>
#include <limits>

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

#define DEBUG(...) \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n");

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
  const char *inName = "substrcmp.in";
  const char *outName = "substrcmp.out";
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


int const maxLen = 100000 + 2;
int const alpha = 256;
char s[maxLen];
int m, strLen;
array<int,20> kPower;

int indexies[maxLen], counter[maxLen], secondIdx[maxLen], classNumNew[maxLen];
int classNum[20][maxLen];

void initPower()
{
  int const enough = 19;
  int k = 1;
  for (int i = 0; i < enough; i++)
    {
      kPower[i] = k;
      k *= 2;
    }
  kPower[enough] = Inf;
}

int findK(int len)
{
  int left = 0;
  int right = 19;
  int mid = (left + right) / 2;
  int p = kPower[mid];
  if (len < p)
    right = mid;
  else if (len > p)
    left = mid;
  else
    return mid;

  for (int i = left; i <= right; i++)
    if (kPower[i] > len)
      return i - 1;
  return 1;
}

int presortCycle()
{
  for (int i = 0; i < alpha; counter[i++] = 0);
  for (int i = 0; i < strLen; i++)
    {
      counter[s[i]]++;
    }
  for (int i = 1; i < alpha; i++)
    {
      counter[i] += counter[i - 1];
    }
  for (int i = 0; i < strLen; i++)
    {
      indexies[--counter[s[i]]] = i;
    }

  classNum[0][indexies[0]] = 0;
  int classes = 1;
  for (int i = 1; i < strLen; i++)
    {
      if (s[indexies[i]] != s[indexies[i - 1]])
        classes++;
      classNum[0][indexies[i]] = classes - 1;
    }

  return classes;
}

void buildSuffixArray()
{
  int classes = presortCycle();

  int layout = 1;
  for (int h = 0; (1 << h) < strLen; h++, layout++)
    {
      for (int i = 0; i < strLen; ++i)
        {
          secondIdx[i] = indexies[i] - (1 << h);
          if (secondIdx[i] < 0)  secondIdx[i] += strLen;
        }

      for (int i = 0; i < classes; counter[i++] = 0);
      for (int i = 0; i < strLen; i++)
        {
          counter[classNum[layout - 1][secondIdx[i]]]++;
        }
      for (int i = 1; i < classes; i++)
        {
          counter[i] += counter[i - 1];
        }
      for (int i = strLen - 1; i >= 0; --i)
        {
          indexies[--counter[classNum[layout - 1][secondIdx[i]]]] = secondIdx[i];
        }

      classNumNew[indexies[0]] = 0;
      classes = 1;
      for (int i = 1; i < strLen; i++)
        {
          int mid1 = (indexies[i] + (1 << h)) % strLen;
          int mid2 = (indexies[i - 1] + (1 << h)) % strLen;
          if (classNum[layout - 1][indexies[i]] != classNum[layout - 1][indexies[i - 1]]
              || classNum[layout - 1][mid1] != classNum[layout - 1][mid2])
            {
              classes++;
            }
          classNumNew[indexies[i]] = classes - 1;
        }
      for (int i = 0; i < strLen; classNum[layout][i] = classNumNew[i], i++);
    }
}



bool isEqual(int a, int b, int len)
{
  int k = findK(len);
  return classNum[k][a] == classNum[k][b]
      && classNum[k][a + len - (1 << k)] == classNum[k][b + len - (1 << k)];
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  initPower();

  scanf("%s", s);
  scanf("%d", &m);
  strLen = strlen(s);
  buildSuffixArray();

  int a,b,c,d;
  bool result = false;
  for (int i = 0; i < m; i++)
    {
      scanf("%d%d%d%d", &a, &b, &c, &d);
      b = b - a + 1;
      d = d - c + 1;
      result = (b == d) && isEqual(--a, --c, b);
      if (result)
        printf("Yes\n");
      else
        printf("No\n");
    }


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}