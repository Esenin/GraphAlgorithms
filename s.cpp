//! @file
//! @author Ivan Senin
//! @brief Problem S. BFS 0-2

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <math.h>
#include <algorithm>
#include <unordered_map>


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
  freopen("island2.in", "r", stdin);
  freopen("island2.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint Inf = numeric_limits<uint>::max();
size_t n = 0;
size_t m = 0;

size_t const NMax = 100 * 1000;

vector<vector<std::pair<uint, uint>>> graph;

vector<uint> dist;
vector<uint> prevv;

// from 0 to (n - 1)
void bfs()
{
  queue<std::pair<uint, uint>> q;
  q.push ( {0, 0} );
  prevv[0] = 0;
  dist[0] = 0;
  while (!q.empty())
    {
      uint v = q.front().first;
      uint wait = q.front().second;
      q.pop();

      if (wait)
        q.push(make_pair(v, wait - 1));
      else
        for (size_t i = 0; i < graph[v].size(); i++)
          {
            uint to = graph[v][i].first;
            uint weight = graph[v][i].second;

            if (dist[v] + weight < dist[to])
              {
                q.push (make_pair(to, weight));
                dist[to] = dist[v] + weight;
                prevv[to] = v;
              }

          }
    }
}




int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  vector<bool> property(n, false);

  graph.resize(n);
  dist.resize(n, Inf);
  prevv.resize(n);

  for (uint i = 0; i < n; i++)
    {
      int p;
      scanf("%d", &p);
      if (p % 2)
        property[i] = true;
    }

  for (uint i = 0; i < m; ++i)
    {
      uint f, t;
      scanf("%d%d", &f, &t);
      f--;
      t--;
      uint dist = (property[f] ^ property[t])? ( ((f + 1) % 2)? 1u : 2u) : 0u;
      graph[f].push_back(make_pair(t, dist));

      dist = (dist)? (  ((t + 1) % 2)? 1u : 2u ) : 0;
      graph[t].push_back(make_pair(f, dist));
    }

  bfs();

  if (dist.back() == Inf)
    {
      printf("impossible\n");
    }
  else
    {
      vector<uint> path;
      for (uint  v = n - 1; v != prevv[v];  v = prevv[v])
        path.push_back (v);
      path.push_back(0);

      printf("%d %d\n", dist.back(), path.size());
      for (size_t i = path.size() - 1; i > 0; i--)
        printf("%d ", (path[i] + 1));
      printf("%d\n", path.front() + 1);
    }



  postActions();
  return 0;
}