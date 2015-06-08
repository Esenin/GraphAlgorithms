//! @author Ivan Senin
//! @brief Cutpoints classic

#include <stdio.h>
#include <vector>
#include <set>
#include <limits>
#include <stdexcept>

using namespace std;

typedef unsigned long long int uint64;
typedef long long int int64;
typedef unsigned int uint;

uint const max_n = 20000;

size_t n = 0;
size_t m = 0;
vector<vector<uint>> graph;
vector<bool> used;
vector<int> tin, fup;
set<uint> cutpoints;

//int64 max_len = numeric_limits<uint>::min();
int timer = 0;


void initSolution()
{
  freopen("points.in", "r", stdin);
  freopen("points.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

void dfs (int v, int p = -1)
{
  used[v] = true;
  tin[v] = fup[v] = timer++;
  int children = 0;
  for (uint i = 0; i < graph[v].size(); i++)
    {
      int to = graph[v][i];
      if (to == p)
        continue;
      if (used[to])
        {
          fup[v] = min (fup[v], tin[to]);
        }
      else
        {
          dfs (to, v);
          fup[v] = min (fup[v], fup[to]);
          if (fup[to] >= tin[v] && p != -1)
            cutpoints.insert(v);
          children++;
        }
    }

  if (p == -1 && children > 1)
    cutpoints.insert(v);
}


int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  graph.resize(n + 1);
  used.resize(n + 1, false);
  tin.resize(n + 1);
  fup.resize(n + 1);

  for (uint i = 0; i < m; ++i)
    {
      uint f, t;
      scanf("%d%d", &f, &t);
      graph[f].push_back(t);
      graph[t].push_back(f);
    }

  dfs(1);

  printf("%d\n", cutpoints.size());
  for (auto &x : cutpoints)
    {
      printf("%d\n", x);
    }

  postActions();
  return 0;
}