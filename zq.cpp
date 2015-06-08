//! @file
//! @author Ivan Senin
//! @brief Problem ZX. Palindrome


#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cstring>
#include <limits>

#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#ifdef HOMERUN
  #include <chrono>
#endif

#ifdef WIN
  #define LLD "%I64d"
#else
  #define LLD "%lld"
#endif

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


void ioInit()
{
#ifndef HOMERUN
  const char *inName = "palindrome.in";
  const char *outName = "palindrome.out";
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


int const maxN = 100 * 1000;
char s[maxN + 1];

int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  scanf("%s", s);
  uint n = strlen(s);

  vector<int64> palinsOdd (n, 0);
  vector<int64> palinsEven(n, 0);
  int64 leftO = 0;
  int64 leftE = 0;
  int64 rightO = -1;
  int64 rightE = -1;
  for (uint i = 0; i < n; i++)
    {
      int64 kO = 1;
      if (i <= rightO)
        {
          kO = min (palinsOdd[leftO + rightO - i], rightO - i);
        }
      while (i + kO < n && i - kO >= 0 && s[i + kO] == s[i - kO])
        kO++;
      palinsOdd[i] = kO--;
      if (i + kO > rightO )
        {
          leftO =  i - kO;
          rightO = i + kO;
        }

      int64 kE = 1;
      if (i <= rightE)
        {
          kE =  min (palinsEven[leftE + rightE - i + 1], rightE - i + 1);
        }
      while (i + kE - 1 < n && i - kE >= 0 && s[i + kE - 1] == s[i - kE])
        kE++;
      palinsEven[i] = --kE;
      if (i + kE - 1 > rightE )
        {
          leftE = i - kE;
          rightE  = i + kE- 1 ;
        }
    }

  auto sum = accumulate(palinsOdd.begin(), palinsOdd.end(), 0ll, [](int64 acc, int64 x)
  {
      return x + acc - 1;
  })
   + accumulate(palinsEven.begin(), palinsEven.end(), 0ll, [](int64 acc, int64 x)
  {
      return (x > 0)? x + acc : acc;
  });

  printf(LLD "\n", sum);


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}//! @file
//! @author Ivan Senin
//! @brief Problem ZX. Palindrome


#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cstring>
#include <limits>

#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#ifdef HOMERUN
  #include <chrono>
#endif

#ifdef WIN
  #define LLD "%I64d"
#else
  #define LLD "%lld"
#endif

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


void ioInit()
{
#ifndef HOMERUN
  const char *inName = "palindrome.in";
  const char *outName = "palindrome.out";
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


int const maxN = 100 * 1000;
char s[maxN + 1];

int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  scanf("%s", s);
  uint n = strlen(s);

  vector<int64> palinsOdd (n, 0);
  vector<int64> palinsEven(n, 0);
  int64 leftO = 0;
  int64 leftE = 0;
  int64 rightO = -1;
  int64 rightE = -1;
  for (uint i = 0; i < n; i++)
    {
      int64 kO = 1;
      if (i <= rightO)
        {
          kO = min (palinsOdd[leftO + rightO - i], rightO - i);
        }
      while (i + kO < n && i - kO >= 0 && s[i + kO] == s[i - kO])
        kO++;
      palinsOdd[i] = kO--;
      if (i + kO > rightO )
        {
          leftO =  i - kO;
          rightO = i + kO;
        }

      int64 kE = 1;
      if (i <= rightE)
        {
          kE =  min (palinsEven[leftE + rightE - i + 1], rightE - i + 1);
        }
      while (i + kE - 1 < n && i - kE >= 0 && s[i + kE - 1] == s[i - kE])
        kE++;
      palinsEven[i] = --kE;
      if (i + kE - 1 > rightE )
        {
          leftE = i - kE;
          rightE  = i + kE- 1 ;
        }
    }

  auto sum = accumulate(palinsOdd.begin(), palinsOdd.end(), 0ll, [](int64 acc, int64 x)
  {
      return x + acc - 1;
  })
   + accumulate(palinsEven.begin(), palinsEven.end(), 0ll, [](int64 acc, int64 x)
  {
      return (x > 0)? x + acc : acc;
  });

  printf(LLD "\n", sum);


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}