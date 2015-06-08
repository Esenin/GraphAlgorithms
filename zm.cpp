//! @file
//! @author Ivan Senin
//! @brief Problem ZM. Dinic Algo. Hardcore


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
  const char *inName = "flow2.in";
  const char *outName = "flow2.out";
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


int const maxN = 1000;
int const maxM = 20001;
int n, m;

struct Edge
{
  int  from;
  int  to;
  int64 c;
  int64 flow;
  Edge* reverse;

  Edge() {}

  Edge* init(int f, int t, int64 cap)
  {
    from = f; to = t; c = cap;
    reverse = nullptr;
    return this;
  }

  int64 rest() const { return c - flow; }
};

Edge edgesPull[2 * maxM + 5];
size_t gSizes[maxN] {};
Edge* graphE[maxN][maxM];
Edge* prevv[2 * maxM];
int q[2 * maxM];
Edge* pipes[maxM];

Edge* const endPathPtr = (Edge *)0x123;

int qhead = 0;
int qtail = 0;

void pushq(int x)
{
  q[qtail++] = x;
  if (qtail == 2 * maxM)
    qtail = 0;
}

int popq()
{
  int res = q[qhead++];
  if (qhead == 2 * maxM)
    qhead = 0;
  return res;
}

void resetq()
{
  qhead = 0;
  qtail = 0;
}

bool makeLayouts()
{
  for (int i = 1; i < n; i++)
    prevv[i] = nullptr;
  prevv[0] = endPathPtr;

  pushq(0);
  while (qhead < qtail)
    {
      int curV = popq();
      for (uint i = 0; i < gSizes[curV]; i++)
        {
          Edge *e = graphE[curV][i];
          if (prevv[e->to] == nullptr && e->rest() > 0)
            {
              prevv[e->to] = e;
              pushq(e->to);
            }
        }
    }
  resetq();
  return prevv[n - 1] != nullptr;
}

int64 findBlockingFlow()
{
  int64 dFlow = 0;
  for (uint i = 0; i < gSizes[n - 1]; i++)
    {
      Edge *startE =  graphE[n - 1][i]->reverse;
      int64 fflow = Inf64;
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
  fprintf(stderr, "struct size %ld bytes\n", sizeof(Edge));
#endif
  ioInit();

  scanf("%d%d", &n, &m);

  for (int i = 0; i < m; i++)
    {
      int f, t, c;
      scanf("%d%d%d", &f, &t, &c);
      --f; --t;
      Edge *fwd = edgesPull[i].init(f, t, c);
      Edge *rev = edgesPull[2 * m + i + 1].init(t, f, 0);
      fwd->reverse = rev;
      rev->reverse = fwd;

      graphE[f][gSizes[f]++] = fwd;
      graphE[t][gSizes[t]++] = rev;

      pipes[i] = fwd;
    }

  int64 res = 0ll;
  while (makeLayouts())
    res += findBlockingFlow();


  printf(LLD"\n", res);

  for (int i = 0; i < m; i++)
    {
      int64 ci = pipes[i]->flow;
      if (ci < 0)
        ci = 0;
#ifdef HOMERUN
      int f = pipes[i]->from;
      int t = pipes[i]->to;
      printf("(%d -> %d): " LLD "/" LLD "\n", f+1, t+1, ci, pipes[i]->c);
#else
      printf(LLD "\n", ci);
#endif
    }

  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}