//! @file
//! @author Ivan Senin
//! @brief Problem ZK. Flow, Ford Falkerson

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <cstring>
#include <limits>

#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#ifdef HOMERUN
  #include <chrono>
#endif

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

#ifndef HOMERUN
  const char *inName = "flow.in";
  const char *outName = "flow.out";
#else
  const char *inName = "input";
  const char *outName = "output";
#endif

  freopen(inName, "r", stdin);
  freopen(outName, "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

int Inf = numeric_limits<int>::max();

int n, m;
vector<vector<int>> graphm, flow;
vector<tuple<int, int, int>> pipes;

vector<int> shortestUnusedPath()
{
  vector<int> from(n, -1);
  queue<int> q;

  q.push(0);
  from[0] = 0;
  while(!q.empty())  // find shortest path
    {
      int cur = q.front();
      q.pop();
      for (int v = 0; v < n; v++)
        if (from[v] == -1 && (graphm[cur][v] - flow[cur][v] > 0))
          {
            q.push(v);
            from[v] = cur;
          }
    }
  return from;
}

int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  initSolution();

  scanf("%d%d", &n, &m);
  graphm.resize(n, vector<int>(n));
  flow.resize(n, vector<int>(n));
  pipes.reserve(m);
  for (int i = 0; i < m; i++)
    {
      int f, t, c;
      scanf("%d%d%d", &f, &t, &c);
      graphm[f - 1][t - 1] += c;
      graphm[t - 1][f - 1] += c;
      pipes.push_back(make_tuple(f - 1, t - 1, c));
    }

  while(true) // while exist unused path from 0 to (n-1)
    {
      auto const path = shortestUnusedPath();
      if (path[n - 1] == -1) // no more path
        break;

      int maxf = Inf;
      for (int cur = n - 1; cur != 0; cur = path[cur]) // max flow on path == min flow per edge
        {
          int prev = path[cur];
          maxf = min (maxf, graphm[prev][cur] - flow[prev][cur]);
        }
      for (int cur = n - 1; cur != 0; cur = path[cur])
        {
          int prev = path[cur];
          flow[prev][cur] += maxf;
          flow[cur][prev] -= maxf;
        }
    }

  int res_flow = 0;
  for (int i = 0; i < n; i++)
    if (graphm[0][i])
      res_flow += flow[0][i];

  printf("%.3f\n", double(res_flow));

  for (auto &xt : pipes)
    {
      int f, t, c;
      tie(f, t, c) = xt;
      if (flow[f][t] < 0)
        c = -c;
      int cmi = min(c, flow[f][t]);
      int cma = max(c, flow[f][t]);
      int const ci = (abs(cma) - abs(cmi) > 0)? cmi : cma;
      flow[f][t] -= ci;
      flow[t][f] += ci;
      printf("%.3f\n", double(ci));
    }



  postActions();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [ms] = %ld\n", duration.count());
#endif
  return 0;
}