//! @author Ivan Senin
//! @brief Topological sort

#include <stdio.h>
#include <vector>
#include <stdexcept>

using namespace std;

typedef unsigned long long int uint64;
typedef unsigned int uint;

size_t n = 0;
size_t m = 0;
vector<vector<int>> graph;
vector<int> ans;
vector<int> colors;

void initSolution()
{
  freopen("topsort.in", "r", stdin);
  freopen("topsort.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

void dfs (int v)
{
  colors[v] = 1;
  for (size_t i = 0; i < graph[v].size(); i++)
    {
      int to = graph[v][i];
      if (colors[to] == 0)
        {
          dfs (to);
        }
      else if (colors[to] == 1)
        throw std::logic_error("cycle!");
    }
  colors[v] = 2;
  ans.push_back (v);
}

void topological_sort()
{
  ans.clear();
  for (uint i = 1; i <= n; i++)
    {
      if (colors[i] == 0)
        dfs (i);
    }
}

int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  graph.resize(n + 1);
  colors.resize(n + 1, 0);
  ans.reserve(n);
  for (uint i = 0; i < m; ++i)
    {
      int f, t;
      scanf("%d%d", &f, &t);
      graph[f].push_back(t);
    }

  bool cycle = false;
  try
  {
    topological_sort();
  }
  catch(std::logic_error &)
  {
    cycle = true;
  }

  if (cycle)
    {
      printf("-1\n");
    }
  else
    {
      for (int i = n - 1; i >= 0; i--)
        printf("%d ", ans[i]);
      printf("\n");
    }

  postActions();
  return 0;
}