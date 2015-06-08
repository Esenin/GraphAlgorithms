//! @file
//! @author Ivan Senin
//! @brief Problem ZB. Nim, Divgame

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <set>
#include <limits>
#include <cstring>
#include <stdexcept>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>

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
  freopen("divgame.in", "r", stdin);
  freopen("divgame.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint Inf = numeric_limits<uint>::max();
size_t w = 0;
size_t h = 0;

size_t const wMax = 100 + 1;
size_t const usedLen = 2000;
size_t const maxInbufLen = 20000;

uint cache[wMax][wMax];
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

uint grundy(uint const wt, uint const ht)
{
  if (wt == 1 || ht == 1)
    {
      cache[wt][ht] = 0;
      return 0;
    }

  //vector<uint> inbuf;
  uint counter = 0;
  for (uint i = 1; i < ht; i++)
    for (uint j = 1; j < wt; j++)
      {
        inbuf[counter++] = (
                            cache[wt][i]
                            ^ cache[j][ht - i]
                            ^ cache[wt - j][ht - i]
        );
        inbuf[counter++] = (
                            cache[j][ht]
                            ^ cache[wt - j][i]
                            ^ cache[wt - j][ht - i]
        );
      }

  uint const result = mex(counter);
  cache[wt][ht] = result;
  return result;
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  initSolution();
  scanf("%d %d", &w, &h);

  version = 0;

  for (uint i = 1; i <= w; i++)
    for (uint j = 1; j <= h; j++)
      grundy(i, j);

  if (cache[w][h])
    {
      printf("Alice\n");
      for (uint i = 1; i < h; i++)
        {
          bool found = false;
          for (uint j = 1; j < w; j++)
            {
              if ((cache[w][i] ^ cache[j][h - i] ^ cache[w - j][h - i]) == 0)
                {
                  printf("0 %d %d %d\n", i, w, i);
                  printf("%d %d %d %d\n", j, i, j, h);
                  found = true;
                  break;
                }
              if ((cache[j][h] ^ cache[w - j][i] ^ cache[w - j][h - i]) == 0)
                {
                  printf("%d 0 %d %d\n", j, j, h);
                  printf("%d %d %d %d\n", j, i, w, i);
                  found = true;
                  break;
                }
            }
          if (found)
            break;
        }

    }
  else
    {
      printf("Bob\n");
    }

  postActions();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [ms] = %ld\n", duration.count());
#endif
  return 0;
}