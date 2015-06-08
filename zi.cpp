//! @file
//! @author Ivan Senin
//! @brief Problem ZI. Dinic Algo.


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
  const char *inName = "snails.in";
  const char *outName = "snails.out";
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


int const maxN = 100001;
int const maxM = 100001;
int n, m;

struct Edge
{
  int  from;
  int  to;
  int c;
  int flow;
  Edge* reverse;

  Edge() {}

  Edge* init(int f, int t, int cap = 1, Edge *rev = nullptr)
  {
    from = f; to = t; c = cap; reverse = rev;
    return this;
  }

  int rest() const { return c - flow; }
};

Edge edgesPull[2 * maxM + 2];
vector<Edge*> graphE[maxN];
Edge* prevv[maxN];
int q[5 * maxM];

Edge* const endPathPtr = (Edge *)0x123;


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

bool makeLayouts(int s, int t)
{
  for (int i = 0; i < n; i++)
    prevv[i] = nullptr;
  prevv[s] = endPathPtr;
  int qhead = 0;
  int qtail = 0;

  q[qtail++] = s;
  while (qhead < qtail)
    {
      int const curV = q[qhead++];
      size_t const lineSize = graphE[curV].size();
      size_t idx = startWith(lineSize);
      for (uint i = 0; i < lineSize; ++i, idx = (idx + 1) % lineSize)
        {
          Edge *e = graphE[curV][idx];
          if (prevv[e->to] == nullptr && e->rest() > 0)
            {
              prevv[e->to] = e;
              q[qtail++] = e->to;
            }
        }
    }
  return prevv[t] != nullptr;
}

int findBlockingFlow(int t)
{
  int dFlow = 0;
  size_t const lineSize = graphE[t].size();
  size_t idx = startWith(lineSize);
  for (uint i = 0; i < lineSize; ++i, idx = (idx + 1) % lineSize)
    {
      Edge *startE =  graphE[t][idx]->reverse;
      int fflow = Inf;
      for (Edge const *e = startE; fflow && e != endPathPtr; e = prevv[e->from])
        {
          if (e == nullptr)
            {
              fflow = 0; break;
            }
          fflow = min(fflow, e->rest());
        }
      if (fflow == 0)
        continue;
      for (Edge *e = startE; fflow && e != endPathPtr; e = prevv[e->from])
        {
          e->flow += fflow;
          e->reverse->flow -= fflow;
        }
      dFlow += fflow;
    }
  return dFlow;
}

int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  int s, t;
  scanf("%d%d%d%d", &n, &m, &s, &t);
  s--; t--;

  int const ave = min(m / n, int(1.25 * sqrt(m)));
  for (int i = 0; i < n; graphE[i++].reserve(ave));

  for (int i = 0; i < m; i++)
    {
      int f, t;
      scanf("%d%d", &f, &t);
      --f; --t;
      Edge *fwd = edgesPull[i].init(f, t);
      Edge *rev = edgesPull[m + i].init(t, f, 0, fwd);
      fwd->reverse = rev;

      graphE[f].push_back(fwd);
      graphE[t].push_back(rev);
    }

  int res = 0;
  while (makeLayouts(s, t))
    {
      res += findBlockingFlow(t);
      if (res >= 2)
        break;
    }

  if (res > 1)
    {
      printf("YES\n");
      for (int j = 0; j < 2; j++)
        {
          int curV = s;
          printf("%d ", curV + 1);
          while (curV != t)
            {
              size_t const lineSize = graphE[curV].size();
              size_t idx = startWith(lineSize);
              for (uint i = 0; i < lineSize; ++i, idx = (idx + 1) % lineSize)
                {
                  Edge *vto = graphE[curV][idx];
                  if (vto->flow <= 0)
                    continue;
                  curV = vto->to;
                  printf("%d ", curV + 1);
                  vto->flow = 0;
                  break;
                }
            }
          printf("\n");
        }
    }
  else
    {
      printf("NO\n");
    }

  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}