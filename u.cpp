//! @file
//! @author Ivan Senin
//! @brief Problem U. Bellman-Ford

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
  freopen("maze.in", "r", stdin);
  freopen("maze.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

struct Edge
{
  uint from, to;
  int value;
  Edge(uint const from = 0, uint const to = 0, int const value = 0)
    : from(from), to(to), value(value) {}
};

size_t n = 0;
size_t m = 0;

vector<vector<int>> graph;
vector<Edge> edges;
vector<int> dist;
vector<int> path;
vector<bool> used;

bool hasPositiveCycleWithExit;

int Inf = 2000 * 10000 + 1;


bool dfs(uint const v)
{
  if (v == n - 1)
    {
      used.assign(n, false);
      return true;
    }
  bool result = false;
  used[v] = true;
  for (uint i = 0; i < graph[v].size(); i++)
    {
      uint to = graph[v][i];
      if (!used[to])
        {
          result = result || dfs(to);
          if (result)
            return true;
        }
    }
  return false;
}


void findMaxLen()
{
  dist[0] = 0;
  int xcycle;
  for (uint i = 0; i < n; i++)
    {
      xcycle = -1;
      for (uint j = 0; j < m; j++)
        if (dist[edges[j].from] > -Inf)
          if (dist[edges[j].from] + edges[j].value > dist[edges[j].to])
            {
              dist[edges[j].to] = min(Inf, dist[edges[j].from] + edges[j].value);
              path[edges[j].to] = edges[j].from;
              xcycle = edges[j].to;
            }
    }

  if (xcycle != -1)
    {
      int cycleNode = xcycle;
      for (uint i = 0; i < n; i++)
        cycleNode = path[cycleNode];

      hasPositiveCycleWithExit = dfs(cycleNode);
    }
}


int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  edges.resize(m);
  dist.resize(n, -Inf);
  path.resize(n, -1);
  graph.resize(n);
  used.resize(n);
  hasPositiveCycleWithExit = false;

  for (uint i = 0; i < m; ++i)
    {
      uint f, t, w;
      scanf("%d%d%d", &f, &t, &w);
      f--;
      t--;
      edges[i] = Edge(f, t, w);
      graph[f].push_back(t);
    }

  findMaxLen();

  if (hasPositiveCycleWithExit)
    printf(":)\n");
  else if (dist[n - 1] == -Inf)
    printf(":(\n");
  else
    printf("%d\n", dist[n - 1]);


  postActions();
  return 0;
}