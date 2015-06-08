//! @file
//! @author Ivan Senin
//! @brief Problem ZG, terminator, retroanalysis

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
  freopen("terminator.in", "r", stdin);
  freopen("terminator.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint Inf = numeric_limits<uint>::max();

int const boardLen = 8;
int const boardSquare = (boardLen + 1) * boardLen;

char board[boardLen + 1][boardLen + 1];

struct Point
{
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
  explicit Point(int abs) : x(abs % boardLen), y(abs / boardLen) {}

  Point& operator+=(Point const &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  bool isValidKillerPos() const
  {
    return (x >= 0) && (x < boardLen) && (y >= 0) && (y < boardLen)
            && board[y][x] != '1';
  }

  bool isValidHostPos()
  {
    return (x >= 0) && (x < boardLen) && (y >= 0) && (y <= boardLen)
            && board[y][x] != '1';
  }

  inline int toAbs() const { return y * boardLen + x; }
};

struct State
{
  Point killer;
  Point hostage;
  bool killerStep;
};

vector<State> graph [boardSquare][boardSquare][2];
bool winpos [boardSquare][boardSquare][2] {};
bool losepos [boardSquare][boardSquare][2] {};
bool used[boardSquare][boardSquare][2] {};
int degree[boardSquare][boardSquare][2] {};


tuple<int, int> findStartPos()
{
  int kPos = 0;
  int hostPos = 0;
  for (int i = 0; i < boardLen; i++)
    for (int j = 0; j < boardLen; j++)
      if (board[i][j] == '3')
        kPos = i * boardLen + j;
      else if (board[i][j] == '2')
        hostPos = i * boardLen + j;
  return make_tuple(kPos, hostPos);
}

bool isHostageInRescueZone(Point const &hostage)
{
  return hostage.y == boardLen;
}

bool isHostageInAim(Point const &killer, Point const &hostage)
{
  if (isHostageInRescueZone(hostage))
    return false;
  bool onDirect = (killer.x == hostage.x) || (killer.y == hostage.y)
      || (abs(killer.x - hostage.x) == abs(killer.y - hostage.y));
  if (onDirect)
    {
      int dx = hostage.x - killer.x, dy = hostage.y - killer.y;
      for (int i = killer.x, j = killer.y; i != hostage.x || j != hostage.y;
              i = (dx == 0)? i : (dx > 0)? i + 1 : i - 1,
              j = (dy == 0)? j : (dy > 0)? j + 1 : j - 1)
        {
          if (board[j][i] == '1')
            return false;
        }
    }
  return onDirect;
}

void fillGraph()
{
  for (int killerPos = 0; killerPos < boardSquare; killerPos++)
    for (int hostagePos = 0; hostagePos < boardSquare; hostagePos++)
      for (int kStep = 0; kStep <= 1; kStep++)
        {
          Point kPoint(killerPos), hPoint(hostagePos);
//          if (killerPos == 8 && hostagePos == 58)
//            {
//              fprintf(stderr, "ololo!!!\n");
//            }
          if (!hPoint.isValidHostPos() || !kPoint.isValidKillerPos())
            continue;

          bool isWinPoint = false;
          bool isLosePoint = false;

          if (kStep)
            {
              isWinPoint = isHostageInAim(kPoint, hPoint);
              isLosePoint = !isWinPoint && isHostageInRescueZone(hPoint);
            }
          else
            {
              isWinPoint = isHostageInRescueZone(hPoint) && !isHostageInAim(kPoint, hPoint);
              isLosePoint = isHostageInAim(kPoint, hPoint);
            }
          winpos[killerPos][hostagePos][kStep] = isWinPoint;
          losepos[killerPos][hostagePos][kStep] = isLosePoint;
          if (isWinPoint || isLosePoint)
            continue;

          // intermediate step
          State nextMove { kPoint, hPoint, !kStep };
          graph[killerPos][hostagePos][kStep].push_back(nextMove);
          nextMove.killerStep = kStep != 0;
          degree[killerPos][hostagePos][kStep] = 1;

          Point mov[] = {Point(-1, 0), Point(0, 1), Point(1, 0), Point(0, -1),
                        Point(-1, -1), Point(-1, 1), Point(1, -1), Point(1, 1)};
          for (int i = 0; i < 8 ; i++)
            {
              Point nk = kPoint;
              Point nh = hPoint;
              if (kStep)
                {
                  nk += mov[i];
                }
              else
                {
                  nh += mov[i];
                }
              if (nk.isValidKillerPos() && nh.isValidHostPos())
                {
                  graph[nk.toAbs()][nh.toAbs()][!kStep].push_back(nextMove);
                  degree[killerPos][hostagePos][kStep]++;
                }
            }
        }
}

void dfs (int terminator, int hostage, bool kStep)
{
  used[terminator][hostage][kStep] = true;
  for (auto it = graph[terminator][hostage][kStep].begin(); it != graph[terminator][hostage][kStep].end(); it++)
    {
      int killerPos = it->killer.toAbs(), hostagePos = it->hostage.toAbs();
      bool nkstep = it->killerStep;
      if (used[killerPos][hostagePos][nkstep])
        continue;

      if (losepos[terminator][hostage][kStep])
        winpos[killerPos][hostagePos][nkstep] = true;
      else if (--degree[killerPos][hostagePos][nkstep] == 0)
        losepos[killerPos][hostagePos][nkstep] = true;
      else
        continue;

      dfs (killerPos, hostagePos, nkstep);
    }
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  initSolution();

  for (uint i = 0; i < boardLen; i++)
        scanf("%s", board[i]);

  fillGraph();

  for (int kPos = 0; kPos < boardSquare; kPos++)
    for (int hostPos = 0; hostPos < boardSquare; hostPos++)
      for (int kstep = 0; kstep <= 1; kstep++)
        if ((winpos[kPos][hostPos][kstep] || losepos[kPos][hostPos][kstep])
            && !used[kPos][hostPos][kstep])
          {
            dfs (kPos, hostPos, kstep);
          }

  int skPos, sHostPos;
  tie(skPos, sHostPos) = findStartPos();

   if (winpos[skPos][sHostPos][false])
    {
      printf("1\n");
    }
  else
    {
      printf("-1\n");
    }


  postActions();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [ms] = %ld\n", duration.count());
#endif
  return 0;
}