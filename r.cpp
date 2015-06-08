//! @file
//! @author Ivan Senin
//! @brief Problem R

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
  freopen("knight2.in", "r", stdin);
  freopen("knight2.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint const n = 65;
uint const startIndex = numeric_limits<uint>::max();

vector<uint> dist, path;


/**************************************************************
 *   adress conversation funs
 *************************************************************/

int c2Coord(char const c)
{
  return c - 'a' + 1;
}

char coord2C(int const x)
{
  return 'a' + (x - 1);
}

/**************************************************************
 *************************************************************/

struct CPoint
{
  int x;
  int y;

  CPoint(int const x, int const y)
    : x(x), y(y) {}

  explicit CPoint(uint const absId)
    : x(((absId % 8)? (absId % 8) : 8))
    , y(absId / 8 + ((absId % 8)? 1 : 0))
  {}

  CPoint(CPoint const &other)
    : x(other.x), y(other.y) {}

  CPoint(string const &pos)
  {
    char xx = pos[0];
    y = pos[1] - '0';
    x = c2Coord(xx);
  }

  CPoint operator+(CPoint const &add) const
  {
    return CPoint(x + add.x, y + add.y);
  }

  CPoint& operator+=(CPoint const &right)
  {
    x += right.x;
    y += right.y;
    return *this;
  }

  operator bool() const
  {
    return (x >= 1) && (x <= 8) && (y >= 1) && (y <= 8);
  }

  bool operator==(CPoint const &r) const
  {
    return (x == r.x) && (y == r.y);
  }

  /*string toStr() const
  {
    string result(1, coord2C(x));
    result += to_string(y);
    return result;
  }*/

  uint toAbs() const
  {
    return (y - 1) * 8 + x;
  }
};


struct CState
{
  CPoint const p1;
  CPoint const p2;

  explicit CState(uint const absId)
    : p1(CPoint((absId / 64) + 1))
    , p2(CPoint((absId % 64) + 1))
  {
  }

  CState(CPoint const &p1, CPoint const &p2)
    : p1(p1)
    , p2(p2)
  {}

  uint toAbs() const
  {
    return (p1.toAbs() - 1) * 64 + p2.toAbs() - 1;
  }

  operator size_t() const
  {
    return toAbs();
  }

  operator bool() const
  {
    return p1 && p2 && !(p1 == p2);
  }

  pair<int, CPoint> diff(CState const &next) const
  {
    if (toAbs() == next.toAbs())
      return make_pair<int, CPoint>(-1, CPoint(1, 1));

    if (p1 == next.p1)
      return make_pair<int, CPoint>(2, CPoint(p2));
    else
      return make_pair<int, CPoint>(1, CPoint(p1));
  }
};



void bfs(CState const &s)
{
  vector<CPoint> possible;
  possible.push_back(CPoint(1 ,2));
  possible.push_back(CPoint(2, 1));
  possible.push_back(CPoint(2, -1));
  possible.push_back(CPoint(1, -2));
  possible.push_back(CPoint(-1, -2));
  possible.push_back(CPoint(-2, -1));
  possible.push_back(CPoint(-2, 1));
  possible.push_back(CPoint(-1, 2));

  queue<CState> q;
  q.push (s);
  path[s] = startIndex;
  dist[s] = 0;

  while (!q.empty())
    {
      CState const curr = q.front();
      q.pop();

      for (CPoint const &mov : possible)
        {
          // for the first horse
          CState nextSt1(curr.p1 + mov, curr.p2);
          if (nextSt1 && dist[curr] + 1 < dist[nextSt1])
            {
              q.push(nextSt1);
              dist[nextSt1] = dist[curr] + 1;
              path[nextSt1] = curr.toAbs();
            }

          // for the second one
          CState nextSt2(curr.p1, curr.p2 + mov);
          if (nextSt2 && dist[curr] + 1 < dist[nextSt2])
            {
              q.push(nextSt2);
              dist[nextSt2] = dist[curr] + 1;
              path[nextSt2] = curr.toAbs();
            }
        }
    }
}

CState readState()
{
  string str;
  char buf[3];
  scanf("%2s", buf);
  str = buf;
  CPoint sp1(str);
  scanf("%2s", buf);
  str = buf;
  CPoint sp2(str);
  return CState(sp1, sp2);
}

int main()
{
  initSolution();

  dist.resize(64 * 64, numeric_limits<int>::max());
  path.resize(64 * 64);

  CState start = readState();
  CState end = readState();

  bfs(start);

  vector<uint> ans;

  for (uint v = end.toAbs(); v != startIndex; v = path[v])
    ans.push_back (v);

  for (uint i = ans.size() - 1; i > 0; i--)
    {
      pair<int, CPoint> const diff = CState(ans[i - 1]).diff(CState(ans[i]));
      printf("%d %c%d\n", diff.first, coord2C(diff.second.x), diff.second.y);
    }

  postActions();
  return 0;
}