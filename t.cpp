//! @author Ivan Senin
//! @brief Dijkstra

#include <stdio.h>
#include <vector>
#include <limits>
#include <stdexcept>

using namespace std;

typedef unsigned long long int uint64;
typedef unsigned int uint;

//uint const Inf = numeric_limits<uint>::max();
uint const Inf = 100001;
size_t n = 0;
size_t m = 0;
uint startNode = 0;
uint finNode = 0;
vector<vector<pair<uint, uint>>> graph;
vector<uint> dist, prevv;
vector<bool> used;


void initSolution()
{
  freopen("distance.in", "r", stdin);
  freopen("distance.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}


int main()
{
  initSolution();
  scanf("%d%d", &n, &m);
  scanf("%d%d", &startNode, &finNode);

  graph.resize(n + 1);
  dist.resize(n + 1, Inf);
  prevv.resize(n + 1, 0);
  used.resize(n + 1, false);
  for (uint i = 0; i < m; ++i)
    {
      uint f, t, w;
      scanf("%d%d%d", &f, &t, &w);
      graph[f].push_back(make_pair(t, w));
      graph[t].push_back(make_pair(f, w));
    }

  dist[startNode] = 0;

  for (uint i = 1; i <= n; i++)
    {
      uint v = 0;
      for (uint j = 1; j <= n; j++)
        {
          if (!used[j] && (v == 0 || dist[j] < dist[v]))
            v = j;
        }

      if (dist[v] == Inf)
        break;
      used[v] = true;

      for (uint j = 0; j < graph[v].size(); j++)
        {
          uint to = graph[v][j].first;
          uint len = graph[v][j].second;
          if (dist[v] + len < dist[to])
            {
              dist[to] = dist[v] + len;
              prevv[to] = v;
            }
        }
    }

  if (dist[finNode] == Inf)
    {
      printf("-1\n");
    }
  else
    {
      vector<uint> result;
      for (uint v = finNode; v != startNode; v = prevv[v])
        result.push_back(v);
      result.push_back(startNode);

      printf("%d\n", dist[finNode]);
      for (int i = result.size() - 1; i >= 0; i--)
        printf("%d ", result[i]);

      printf("\n");
    }


  postActions();
  return 0;
}