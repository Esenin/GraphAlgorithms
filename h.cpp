//! @file
//! @author Ivan Senin
//! @brief Problem H. russian reversal

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <limits>
#include <stdexcept>
#include <math.h>

#ifdef WIN32
  #define LLD "%I64d"
#else
  #define LLD "%lld"
#endif

using namespace std;

typedef unsigned long long int uint64;
typedef long long int int64;
typedef unsigned int uint;


uint n = 0;
uint m = 0;


void initSolution()
{
  freopen("reverse.in", "r", stdin);
  freopen("reverse.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}


class Treap
{
public:
  Treap(uint maxSize)
  {
    mNodesPull = new Node[maxSize];
  }

  ~Treap()
  {
    delete[] mNodesPull;
  }

  void add(int const x)
  {
    Node *newElem = &mNodesPull[mCounter];
    newElem->value = x;
    insert(mRoot, newElem, mCounter++);
  }

  int64 sumOfInterval(uint const left, uint const right)
  {
    return getSum(left, right);
  }

  void reverseInterval(uint const left, uint const right)
  {
    reverse(left, right);
  }

protected:
  struct Node;
  Node *mRoot = nullptr;
  Node *mNodesPull = nullptr;
  uint mCounter = 0;

struct Node
{
  int prior;
  int value = 0;
  uint size = 0u;
  int64 sum = 0;
  bool reversed = false;
  Node *l = nullptr;
  Node *r = nullptr;

  Node()
    : prior(rand() % numeric_limits<int>::max())
  {}
};


int size(Node *el)
{
  return el? el->size : 0;
}

void updateSize(Node *el)
{
  if (el)
    el->size = size(el->l) + size(el->r) + 1;
}

int64 sum(Node *el)
{
  return el? el->sum : 0;
}

void updateSum(Node *el)
{
  if (el)
    el->sum = sum(el->l) + sum(el->r) + el->value;
}

void pushReverse(Node *el)
{
  if (el && el->reversed)
    {
      el->reversed = false;
      swap (el->l, el->r);
      if (el->l)
        el->l->reversed ^= true;
      if (el->r)
        el->r->reversed ^= true;
    }
}

void merge(Node* &t, Node *l, Node *r)
{
  pushReverse (l);
  pushReverse (r);
  if (!l || !r)
    t = l ? l : r;
  else if (l->prior > r->prior)
    {
      merge (l->r, l->r, r);
      t = l;
    }
  else
    {
      merge (r->l, l, r->l);
      t = r;
    }
  updateSize(t);
  updateSum(t);
}

void split(Node *t, Node* &l, Node* &r, uint const key, int add = 0)
{
  if (!t)
    {
      l = r = nullptr;
      return;
    }
  pushReverse (t);
  uint cur_key = add + size(t->l);
  if (key <= cur_key)
    {
      split (t->l, l, t->l, key, add);
      r = t;
    }
  else
    {
      split (t->r, t->r, r, key, add + 1 + size(t->l));
      l = t;
    }
  updateSize(t);
  updateSum(t);
}

void reverse(uint l, uint r)
{
  Node *t1;
  Node *t2;
  Node *t3;
  split (mRoot, t1, t2, l);
  split (t2, t2, t3, r - l + 1);
  t2->reversed ^= true;
  merge (mRoot, t1, t2);
  merge (mRoot, mRoot, t3);
}

void insert(Node* &t, Node *newItem, uint const pos)
{
  Node *t1;
  Node *t2;
  split (t, t1, t2, pos);
  merge (t1, t1, newItem);
  merge (t, t1, t2);
  updateSum(t);
}

int64 getSum(uint const l, uint const r)
{
  Node *t1;
  Node *t2;
  Node *t3;
  split(mRoot, t1, t2, l);
  split(t2, t2, t3, r - l + 1);
  int64 const result = t2->sum;
  merge(mRoot, t1, t2);
  merge(mRoot, mRoot, t3);
  return result;
}

};


int main()
{
  initSolution();

  int const maxN = 200 * 1000;
  scanf("%d%d", &n, &m);
  Treap treap(maxN);

  for (uint i = 0; i < n; i++)
    {
      uint h;
      scanf("%u", &h);
      treap.add(h);
    }

  for (uint i = 0; i < m; i++)
    {
      uint q, l, r;
      scanf("%d%d%d", &q, &l, &r);
      if (q)
        {
          treap.reverseInterval(l - 1, r - 1);
        }
      else
        {
          int64 const sum = treap.sumOfInterval(l - 1, r - 1);
          printf(LLD "\n", sum);
        }
    }


  postActions();
  return 0;
}