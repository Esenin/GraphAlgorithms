//! @file
//! @author Ivan Senin
//! @brief Problem ZL. Kuhn algo


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
  const char *inName = "pairs.in";
  const char *outName = "pairs.out";
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


int const maxN = 251;
int const maxM = 251;
int n, m;

vector<int> graphE[maxN]{};
bool used[maxN];
int opp[maxM];


bool dfs(int v)
{
  used[v] = true;
  size_t const lineSize = graphE[v].size();
  size_t idx = startWith(lineSize);
  for (uint i = 0; i < lineSize; ++i, idx = (idx + 1) % lineSize)
    {
      int to = graphE[v][idx];
      if (opp[to] == -1 || (!used[opp[to]] && dfs(opp[to])))
        {
          opp[to] = v;
          return true;
        }
    }
  return false;
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  scanf("%d%d", &n, &m);
  int const ave = int(m / 3.0 * 2.0);
  for (int i = 0; i < n; graphE[i++].reserve(ave));

  for (int i = 0; i < n; i++)
    {
      int to = 0;
      while (true)
        {
          scanf("%d", &to);
          if (!to)
            break;
          graphE[i].push_back(to - 1);
        }
    }

  for (int i = 0; i < m; opp[i++] = -1);

  int counter = 0;

  size_t idx = startWith(n);
  for (int i = 0; i < n; ++i, idx = (idx + 1) % n)
    {
      for (int j = 0; j < n; used[j++] = false);
      if (dfs(idx))
        counter++;
    }

  printf("%d\n", counter);

  for (int i = 0; i < m; i++)
    if (opp[i] != -1)
      printf("%d %d\n", opp[i] + 1, i + 1);



  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}