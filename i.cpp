//! @author Ivan Senin
//! @brief Connections

#include <stdio.h>
#include <vector>
#include <stdexcept>

using namespace std;

typedef unsigned long long int uint64;
typedef unsigned int uint;

size_t n = 0;
size_t m = 0;
vector<vector<int>> graph;
vector<int> connect;

void initSolution()
{
  freopen("connect.in", "r", stdin);
  freopen("connect.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

void dfs (int v, int const index)
{
  connect[v] = index;
  for (size_t i = 0; i < graph[v].size(); i++)
    {
      int to = graph[v][i];
      if (connect[to] == 0)
        {
          dfs (to, index);
        }
    }
}

int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  graph.resize(n + 1);
  connect.resize(n + 1, 0);
  for (uint i = 0; i < m; ++i)
    {
      int f, t;
      scanf("%d%d", &f, &t);
      graph[f].push_back(t);
      graph[t].push_back(f);
    }

  int colors = 0;
  for (uint i = 1; i <= n; i++)
    {
      if (!connect[i])
        dfs(i, ++colors);
    }

  printf("%d\n", colors);
  for (uint i = 1; i <= n; i++)
    {
      printf("%d ", connect[i]);
    }
  printf("\n");



  postActions();
  return 0;
}