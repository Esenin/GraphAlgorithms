//! @author Ivan Senin
//! @brief Cycles

#include <stdio.h>
#include <vector>
#include <stdexcept>

using namespace std;

typedef unsigned long long int uint64;
typedef unsigned int uint;

size_t n = 0;
size_t m = 0;
vector<vector<int>> graph;
vector<int> color;
vector<int> path;
int cycle_start = 0;
int cycle_end = 0;

void initSolution()
{
  freopen("cycle.in", "r", stdin);
  freopen("cycle.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

bool dfs (int v)
{
  color[v] = 1;
  for (size_t i = 0; i < graph[v].size(); i++)
    {
      int to = graph[v][i];
      if (color[to] == 0)
        {
          path[to] = v;
          if (dfs (to))
              return true;
        }
      else if (color[to] == 1)
        {
          cycle_end = v;
          cycle_start = to;
          return true;
        }
    }
  color[v] = 2;
  return false;
}

int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  graph.resize(n + 1);
  color.resize(n + 1, 0);
  path.resize(n + 1, 0);
  for (uint i = 0; i < m; ++i)
    {
      int f, t;
      scanf("%d%d", &f, &t);
      graph[f].push_back(t);
    }

  for (uint i = 1; i <= n; i++)
    {
      if (dfs(i))
        break;
    }

  if (cycle_start == cycle_end)
    {
      printf("NO\n");
    }
  else
    {
      printf("YES\n");
      vector<int> cycle;
      for (int v = cycle_end; v != cycle_start; v = path[v])
        cycle.push_back(v);
      cycle.push_back(cycle_start);

      for (int i = cycle.size() - 1; i >= 0; i--)
        printf("%d ", cycle[i]);

      printf("\n");
    }

  postActions();
  return 0;
}