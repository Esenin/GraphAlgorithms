//! @file
//! @author Ivan Senin
//! @brief Problem ZJ. Min cur


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
  const char *inName = "cut.in";
  const char *outName = "cut.out";
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


int const maxN = 101;
int const maxM = 401;
int n, m;

struct Edge
{
  int  from;
  int  to;
  int c;
  int flow;
  int id;
  bool used;
  Edge* reverse;

  Edge() {}

  Edge* init(int f, int t, int cap, int id_, Edge *rev = nullptr)
  {
    from = f; to = t; c = cap; id = id_; reverse = rev;
    used = false;
    return this;
  }

  int rest() const { return c - flow; }
};

Edge edgesPull[4 * maxM + 2];
vector<Edge*> graphE[maxN];
Edge* prevv[maxN];
int q[10 * maxM];
Edge* pipes[maxM];
vector<int> indices;

bool used[maxN]{};

Edge* const endPathPtr = (Edge *)0x123;


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
  return prevv[t] != nullptr && prevv[t] != endPathPtr;
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

void dfs(int v)
{
  used[v] = true;

  int const lineSize = graphE[v].size();
  int idx = startWith(lineSize);
  for (int i = 0; i < lineSize; i++, idx = (idx + 1) % lineSize)
    {
      Edge *e = graphE[v][idx];
      if (!used[e->to] && e->rest() > 0)
        {
          dfs(e->to);
        }
    }
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
  int const s = 0;
  int const t = n - 1;

  for (int i = 0; i < m; i++)
     {
       int f, t, c;
       scanf("%d%d%d", &f, &t, &c);
       --f; --t;
       Edge *fwd = edgesPull[i].init(f, t, c, i);
       Edge *rev = edgesPull[m + i].init(t, f, 0, 0, fwd);
       fwd->reverse = rev;
       graphE[f].push_back(fwd);
       graphE[t].push_back(rev);
       pipes[i] = fwd;

       fwd = edgesPull[2 * m +i].init(t, f, c, i);
       rev = edgesPull[3 * m + i].init(f, t, 0, 0, fwd);
       fwd->reverse = rev;
       graphE[t].push_back(fwd);
       graphE[f].push_back(rev);
     }


  int res = 0;
  while (makeLayouts(s, t))
    {
      res += findBlockingFlow(t);
    }

  dfs(s);

  indices.reserve(m);

  for (int i = 0; i < m; i++)
    {
      auto e = pipes[i];
      if (used[e->from] ^ used[e->to])
        indices.push_back(i);
    }

  printf("%d %d\n", indices.size(), res);
  sort(indices.begin(), indices.end());
  for (uint i = 0; i < indices.size(); i++)
    printf("%d ", indices[i] + 1);
  printf("\n");

  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}