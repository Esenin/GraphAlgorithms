//! @author Ivan Senin
//! @brief Deepest path

#include <stdio.h>
#include <vector>
#include <limits>
#include <stdexcept>

using namespace std;

typedef unsigned long long int uint64;
typedef unsigned int uint;

//uint const Inf = numeric_limits<uint>::max();
size_t n = 0;
size_t m = 0;
vector<vector<uint>> graph;
vector<uint> len;
vector<bool> used;
uint result = 0;


void initSolution()
{
  freopen("longpath.in", "r", stdin);
  freopen("longpath.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint dfs(uint v)
{
  used[v] = true;
  uint size = 0;
  for (uint j = 0; j < graph[v].size(); j++)
    {
      if (used[graph[v][j]])
        {
          size = max(size, 1 + len[graph[v][j]]);
        }
      else
        {
          size = max(size, 1 + dfs(graph[v][j]));
        }
    }
  len[v] = size;
  result = max(result, size);
  return size;
}


int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  graph.resize(n + 1);
  used.resize(n + 1, false);
  len.resize(n + 1, 0);
  for (uint i = 0; i < m; ++i)
    {
      uint f, t;
      scanf("%d%d", &f, &t);
      graph[f].push_back(t);
    }

  for (uint i = 1; i <= n; i++)
    if (!used[i])
      dfs(i);

  printf("%d\n", result);


  postActions();
  return 0;
}