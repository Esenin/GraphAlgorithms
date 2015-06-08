//! @file
//! @author Ivan Senin
//! @brief Problem X. Kruskal Algo via DSU

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <math.h>
#include <algorithm>


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
  freopen("spantree2.in", "r", stdin);
  freopen("spantree2.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint n = 0;
uint m = 0;

struct Edge
{
  uint v1, v2;
  uint w;

  Edge(){}

  Edge(uint const b, uint const e, uint const w)
    : v1(b), v2(e), w(w) {}

  inline bool operator < (Edge const &other) const
  { return w < other.w; }
};

vector<Edge> graph;
vector<uint> dsuSet;

inline uint getter(uint const v)
{
  uint res = (v == dsuSet[v])? v : getter(dsuSet[v]);
  dsuSet[v] = res;
  return res;
}

void concatSet(uint const a, uint const b)
{
  uint p1 = getter(a);
  uint p2 = getter(b);
  if (a + b % 2)
    swap(p1, p2);
  if (p1 != p2)
    dsuSet[p1] = p2;
}


int main()
{
  initSolution();

  scanf("%d%d", &n, &m);
  graph.resize(m);
  dsuSet.resize(n);

  for (uint i = 0; i < m; i++)
    {
      uint b, e, w;
      scanf("%d%d%d", &b, &e, &w);
      graph[i] = Edge(b - 1, e - 1, w);
    }
  sort(graph.begin(), graph.end());

  uint64 answer = 0;

  for (uint i = 0; i < n; i++)
    dsuSet[i] = i;

  for (uint i = 0; i < m; i++)
    {
      uint v1 = graph[i].v1;
      uint v2 = graph[i].v2;
      if (getter(v1) != getter(v2))
        {
          answer += graph[i].w;
          concatSet (v1, v2);
        }
    }

  printf(LLD "\n", answer);

  postActions();
  return 0;
}