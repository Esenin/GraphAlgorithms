//! @file
//! @author Ivan Senin
//! @brief Problem W. Prim Algo

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
  freopen("unionday.in", "r", stdin);
  freopen("unionday.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint n = 0;
uint const maxN = 5000;
double const Inf = numeric_limits<double> ::max();
int const undef = -1;

vector<vector<double>> graph;

struct VPoint
{
  int x;
  int y;

  VPoint() {}

  VPoint(int const x, int const y)
    : x(x), y(y) {}
};

inline double dist(double const x1, double const y1, double const x2, double const y2)
{
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector<VPoint> vertices;

int main()
{
  initSolution();

  scanf("%d", &n);
  vertices.resize(n);
  graph.resize(n);

  double answer = 0;

  for (uint i = 0; i < n; i++)
    {
      int x, y;
      scanf("%d%d", &x, &y);
      vertices[i] = VPoint(x, y);
      graph[i].resize(n);
    }

  for (uint i = 0; i < n - 1; i++)
    {
      graph[i][i] = Inf;
      for (uint j = i + 1; j < n; j++)
        {

          double const len = dist(vertices[i].x, vertices[i].y, vertices[j].x, vertices[j].y);
          graph[i][j] = len;
          graph[j][i] = len;
        }
    }
  graph[n - 1][n - 1] = Inf;

  vector<bool> used(n);
  vector<double> shortest(n, Inf);
  vector<int> prev(n, undef);
  shortest[0] = 0;

  for (uint i = 0; i < n; i++)
    {
      int v = undef;
      for (int j = 0; j < n; j++)
        {
          if (!used[j] && (v == undef || shortest[j] < shortest[v]))
            v = j;
        }

      used[v] = true;
      if (prev[v] != undef)
        answer += graph[v][prev[v]];

      for (uint to = 0; to < n; to++)
        if (graph[v][to] < shortest[to])
          {
            shortest[to] = graph[v][to];
            prev[to] = v;
          }
    }

  printf("%f\n", answer);

  postActions();
  return 0;
}