//! @file
//! @author Ivan Senin
//! @brief Ave on intervals via segment tree

#include <stdio.h>
#include <vector>
#include <limits>
#include <stdexcept>
#include <math.h>

using namespace std;

typedef unsigned long long int uint64;
typedef long long int int64;
typedef unsigned int uint;


size_t n = 0;
size_t m = 0;
int64 isum = 0;

struct Node
{
  int64 sumEl;
  int64 updEl;
  bool marked = false;
  Node(int64 const &val)
  {
    sumEl = val;
    updEl = 0;
    marked = false;
  }

  Node()
  {
    sumEl = 0;
    updEl = 0;
    marked = 0;
  }
};

vector<Node> segTree; // first for the sum, second for the update


void initSolution()
{
  freopen("middle.in", "r", stdin);
  freopen("middle.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

inline uint lc(uint const &v)
{
  return v << 1;
}

inline uint rc(uint const &v)
{
  return (v << 1) + 1;
}

void buildSegTree (vector<int64> const &base, uint const v, uint const tleft, uint const tright)
{
  if (tleft == tright)
    {
      segTree[v].sumEl = base[tleft];
      segTree[v].updEl = base[tleft];
    }
  else
    {
      uint tm = (tleft + tright) >> 1;
      buildSegTree (base, lc(v), tleft, tm);
      buildSegTree (base, rc(v), tm + 1, tright);
      segTree[v].sumEl = segTree[lc(v)].sumEl + segTree[rc(v)].sumEl;
    }
}

void push (uint v, uint tl, uint tr)
{
  if (segTree[v].marked)
    {
      segTree[v].marked = false;
      segTree[lc(v)].updEl = segTree[rc(v)].updEl = segTree[v].updEl;
      segTree[lc(v)].marked = true;
      segTree[rc(v)].marked = true;
      // update sum:
      uint tmid = (tl + tr) >> 1;
      segTree[lc(v)].sumEl = segTree[v].updEl * (tmid - tl + 1);
      segTree[rc(v)].sumEl = segTree[v].updEl * (tr - tmid);
    }
}

int64 sumHelper (uint v, uint tl, uint tr, uint lb, uint rb)
{
  if (lb > rb)
    {
      return 0; // WAT
    }

  if (lb == tl && rb == tr)
    {
      return segTree[v].sumEl;
    }
  push(v, tl, tr);
  uint tmid = (tl + tr) >> 1;
  int64 result = sumHelper (lc(v), tl, tmid, lb, min(rb, tmid))
                + sumHelper (rc(v), tmid + 1, tr, max(lb, tmid + 1), rb);
  segTree[v].sumEl = segTree[lc(v)].sumEl + segTree[rc(v)].sumEl;
  return result;
}


void updateH (uint v, uint tl, uint tr, uint const lb, uint const rb, int64 const &newVal)
{
  if (lb > rb)
    {
      return;
    }

  if (lb == tl && tr == rb)
    {
      segTree[v].updEl = newVal;
      segTree[v].marked = true;
      segTree[v].sumEl = (tr - tl + 1) * newVal;
    }
  else
    {
      push (v, tl, tr);
      uint tmid = (tl + tr) >> 1;
      updateH (lc(v), tl, tmid, lb, min(rb, tmid), newVal);
      updateH (rc(v), tmid + 1, tr, max(lb, tmid + 1), rb, newVal);
      segTree[v].sumEl = segTree[lc(v)].sumEl + segTree[rc(v)].sumEl;
    }
}

int64 getH (uint v, uint tl, uint tr, uint pos)
{
  if (tl == tr)
    return segTree[v].updEl;

  push (v, tl, tr);

  uint tm = (tl + tr) >> 1;
  int64 result = (pos <= tm)? getH (lc(v), tl, tm, pos) : getH (rc(v), tm + 1, tr, pos);
  segTree[v].sumEl = segTree[lc(v)].sumEl + segTree[rc(v)].sumEl;
  return result;
}


int64 getSum(uint const &leftBound, uint const &rightBound)
{
  return sumHelper(1, 0, n - 1, leftBound - 1, rightBound - 1);
}

int64 get(uint const pos)
{
  return getH(1, 0, n - 1, pos - 1);
}

void update(uint const left, uint const right, int64 const &newVal)
{
  updateH(1, 0, n - 1, left - 1, right - 1, newVal);
}

int main()
{
  initSolution();
  scanf("%d%d", &n, &m);

  segTree.resize(4 * n);
  vector<int64> initData;
  initData.reserve(n);
  for (uint i = 0; i < n; i++)
    {
      int64 x;
      scanf("%I64d", &x);
      initData.push_back(x);
    }
  buildSegTree(initData, 1, 0, n - 1);

  isum = getSum(1, n);

  for (uint i = 0; i < m; i++)
    {
      uint l ,r;
      scanf("%d%d", &l, &r);
      int64 newValue = 0;
      if (getSum(1, n) <= isum)
        {
          newValue = static_cast<int64>(ceil(getSum(l, r) / double(r - l + 1)));
        }
      else
        {
          newValue = static_cast<int64>(floor(getSum(l, r) / double(r - l + 1)));
        }
      update(l, r, newValue);
    }

  for (uint i = 1; i <= n; i++)
    printf("%I64d ", get(i));
  printf("\n");

  postActions();
  return 0;
}