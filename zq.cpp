//! @file
//! @author Ivan Senin
//! @brief Problem ZQ. Least Common Ancestor, Tarjan
//!

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
 *                             Usings & Typedefs
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
  const char *inName = "lca.in";
  const char *outName = "lca.out";
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

int const maxN = 50 * 1000 + 1;

struct Request
{
  int from;
  int to;
  int lca;

  int other(int v)
  { return (v == from)? to : from; }

  Request() {}
  Request(int a, int b) : from(a), to(b), lca(-1) {}
};


vector<int> graphE[maxN];
vector<Request*> queries[maxN];
int dsu[maxN], ancestor[maxN];
bool used[maxN] {};

Request requestsPull[maxN];

int n, m;

int getTop(int v)
{
  if (dsu[v] != v)
    {
      dsu[v] = getTop(dsu[v]);
    }
  return dsu[v];
}

void unite(int a, int b, int head)
{
  a = getTop(a);
  b = getTop(b);
  if (rand() % 2)
    swap(a, b);
  dsu[a] = b;
  ancestor[b] = head;
}

void dfs (int v)
{
  dsu[v] = v;
  ancestor[v] = v;

  used[v] = true;

  forr(graphE[v].size())
  {
    if (!used[graphE[v][i]])
      {
        dfs(graphE[v][i]);
        unite(v, graphE[v][i], v);
      }
  }

  for (int i = 0; i < (int) queries[v].size(); i++)
    {
      auto &q = queries[v][i];
      int to = q->other(v);
      if (used[to] && q->lca == -1)
        q->lca =  ancestor[getTop(to)];
    }
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  scanf("%d", &n);
  for (int i = 1; i < n; i++)
    {
      int prev;
      scanf("%d", &prev);
      graphE[--prev].push_back(i);
    }

  scanf("%d", &m);
  for (int i = 0; i < m; i++)
    {
      int a, b;
      scanf("%d %d", &a, &b);
      --a; --b;
      Request& q = requestsPull[i];
      q = Request(a, b);
      queries[a].push_back(&q);
      queries[b].push_back(&q);
    }

  dfs(0);

  for (int i = 0; i < m; i++)
    printf("%d\n", requestsPull[i].lca + 1);


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}