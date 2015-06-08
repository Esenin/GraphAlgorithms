//! @file
//! @author Ivan Senin
//! @brief Problem ZB. Nim, Divgame

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <cstring>
#include <limits>

#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>


#ifdef HOMERUN
  #include <chrono>
#endif


#ifdef WIN32
  #define LLD "%I64d"
#else
  #define LLD "%lld"
#endif

using namespace std;

typedef unsigned long long int uint64;
typedef long long int int64;
typedef unsigned int uint;

void initSolution()
{
  freopen("varnim.in", "r", stdin);
  freopen("varnim.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint Inf = numeric_limits<uint>::max();
size_t n = 0;


size_t const wMax = 100 + 1;
size_t const usedLen = 2000;
size_t const maxInbufLen = 20000;

uint cache[wMax];
uint used[usedLen];

uint inbuf[maxInbufLen];

uint version = 0;


uint mex(uint const count)
{
  version++;

  for (uint i = 0; i < count; i++)
    {
#ifdef HOMERUN
      assert(inbuf[i] < usedLen);
#endif
      used[inbuf[i]] = version;
    }

  for (uint i = 0; i < count ; ++i)
    if (used[i] != version)
      return i;
  return count;
}

uint grundy(uint const nn)
{
  if (nn == 0)
    {
      //cache[nn] = 0;
      return 0;
    }
  else if (nn == 1)
    {
      //cache[nn] = 1;
      return 1;
    }

  if (nn % 4 == 1 || nn % 4 == 2)
    return nn;
  else if(nn % 4 == 3)
    return nn + 1;
  else
    return nn - 1;


  /*uint counter = 0;

  for (uint i = 0; i < nn; i++)
    {
      inbuf[counter++] = cache[i];
    }
  for (uint i = 1; i < nn; i++)
    {
      inbuf[counter++] = cache[i] ^ cache[nn - i];
    }

  uint const result = mex(counter);
  cache[nn] = result;
  return result;*/
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  initSolution();
  scanf("%d", &n);

  version = 0;


  for (uint i = 1; i <= 100; i++)
      grundy(i);

  for (uint i = 0; i < n; i++) // several tests
    {
      uint k;
      scanf("%d", &k);
      uint a;
      for (uint ki = 0; ki < k; ki++)
        {
          uint x;
          scanf("%d", &x);
          a = (ki == 0)? grundy(x) : a ^ grundy(x);
        } // input
      if (a)
        {
          printf("FIRST\n");
        }
      else
        {
          printf("SECOND\n");
        }
    } // tests

  postActions();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [ms] = %ld\n", duration.count());
#endif
  return 0;
}