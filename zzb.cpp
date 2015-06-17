//! @file
//! @author Ivan Senin
//! @brief Problem ZZB. suffix array in O(N * log N )
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

#define forr(size_) \
  size_t const _lineSize_ = size_; \
  size_t i = startWith(_lineSize_); \
  for (uint _i_ = 0; _i_ < _lineSize_; ++_i_, i = (i + 1) % _lineSize_)


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
  const char *inName = "suffarray.in";
  const char *outName = "suffarray.out";
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
char s[maxLen];
int m, n, layout;
int ks[20];

array<int, maxLen> classes, tempClasses, order, counter, sufArray[20];



void initPower()
{
  int const enough = 18;
  int k = 1;
  for (int i = 0; i < enough; i++)
    {
      ks[i] = k;
      k *= 2;
    }
  ks[enough] = Inf;
}

int findK(int len)
{
  int l = 0;
  int r = 9;
  if (len > ks[r])
    {
      l = r + 1;
      r = 18;
    }
  for (; l < r; l++)
    if (ks[l] > len)
      return l - 1;
  //assert(false);
  return 1;
}


void buildSuffixArray()
{
  layout = 0;
  for (int i = 0; i < n; i++)
    order[i] = n - 1 - i;

  stable_sort(order.begin(), order.begin() + n, [&] (int a, int b) { return s[a] < s[b]; });

  for (int i = 0; i < n; i++)
    {
      sufArray[layout][i] = order[i];
      classes[i] = s[i];
    }

  for (int len = 1; len < n; len *= 2)
    {

      for (int i = 0; i < n; tempClasses[i] = classes[i], i++);
      for (int i = 0; i < n; i++)
        {
          classes[sufArray[layout][i]] = i > 0 &&
              tempClasses[sufArray[layout][i - 1]] == tempClasses[sufArray[layout][i]] &&
              sufArray[layout][i - 1] + len < n &&
              tempClasses[sufArray[layout][i - 1] + len / 2] == tempClasses[sufArray[layout][i] + len / 2] ?
                classes[sufArray[layout][i - 1]] : i;
        }


      for (int i = 0; i < n; i++)
        {
          counter[i] = i;
          sufArray[layout + 1][i] = sufArray[layout][i];
        }

      for (int i = 0; i < n; i++)
        {
          int s1 = sufArray[layout][i] - len;
          if (s1 >= 0)
              sufArray[layout + 1][counter[classes[s1]]++] = s1;

        }
      layout++;
    }
}



bool isEqual(int a, int b, int len)
{
  if (len == 1)
    return s[a] == s[b];
  int k = findK(len);
  return sufArray[k][a] == sufArray[k][b]
      && sufArray[k][a + len - (1 << k)] == sufArray[k][b + len - (1 << k)];
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  initPower();
//  scanf("%s", s);
//  scanf("%d", &m);
  fgets(s, maxLen, stdin);
  n = strlen(s);
  if (s[n - 1] == '\n')
    {
      s[n - 1] = '\0';
      n--;
    }
  buildSuffixArray();

//  int a,b,c,d;
//  bool result = false;
//  for (int i = 0; i < m; i++)
//    {
//      scanf("%d%d%d%d", &a, &b, &c, &d);
//      b = b - a + 1;
//      d = d - c + 1;
//      result = (b == d) && isEqual(--a, --c, b);
//      if (result)
//        printf("Yes\n");
//      else
//        printf("No\n");
//    }

  for (int i = 0; i < n; i++)
    printf("%d ", sufArray[layout][i] + 1);
  printf("\n");


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}