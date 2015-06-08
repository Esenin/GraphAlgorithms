//! @file
//! @author Ivan Senin
//! @brief Problem ZZG, Suffix tree
//!

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cstring>
#include <limits>
#include <functional>

#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <array>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

/**************************************************************************************************
 *                             Macros
 * ***********************************************************************************************/

#ifdef HOMERUN
  #include <chrono>
#endif

#ifdef WIN
  #define LLD "%I64d"
#else
  #define LLD "%lld"
#endif

#define FORR(size_) \
  size_t const _lineSize_ = size_; \
  size_t i = startWith(_lineSize_); \
  for (uint _i_ = 0; _i_ < _lineSize_; ++_i_, i = (i + 1) % _lineSize_)

#define DEBUG(...) { \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); }

/**************************************************************************************************
 *                             Typedefs & Constants
 * ***********************************************************************************************/

using namespace std;

typedef unsigned long long int uint64;
typedef long long int int64;
typedef unsigned int uint;


template<class T>
using wector = vector<vector<T>>;

constexpr int64 Inf64 = numeric_limits<int64>::max();
constexpr int Inf = numeric_limits<int>::max();
constexpr uint uInf = numeric_limits<uint>::max();
constexpr uint64 uInf64 = numeric_limits<uint64>::max();

/**************************************************************************************************
 *                             Common Helpers
 * ***********************************************************************************************/

void ioInit()
{
#ifndef HOMERUN
  const char *inName = "suftree.in";
  const char *outName = "suftree.out";
#else
  const char *inName = "input";
  const char *outName = "output";
#endif

  freopen(inName, "r", stdin);
  freopen(outName, "w", stdout);
}

void ioClose()
{
  fclose(stdin);
  fclose(stdout);
}

//!
//! @brief randomm
//! @author Ivan Senin
//! @param x unsigned int as seed
//! @return unsigned int less that 100k + 3
size_t randomm(size_t x)
{
  static size_t prevRand = 0;
  x = x << (x & 15);
  x = x * prevRand;
  x = x | (~x&(x + 1));
  prevRand = x % 100003; //prime
  return prevRand;
}

size_t startWith(size_t size)
{
  if (!size)
    return 0;
  else if (size == 1)
    return 0;
  return randomm(size) % size;
}

/**************************************************************************************************
 *                             Solution goes here
 * ***********************************************************************************************/
int const maxLen = 100000 + 2;
int const alpha = 27;
char s[maxLen];

int n;
int nodesCount;
int64 substrNum;

struct Node
{
  int left, right, depth, dfsId;
  Node *parent;
  Node *suffLink;
  array<Node*, alpha> children;

  Node() {} // fast static allocation

  Node* init (int l, int r, int d, Node *p)
  {
    left = l;
    right = r;
    depth = d;
    dfsId = -1;
    this->parent = p;
    fill(children.begin(), children.end(), nullptr);
    return this;
  }
};

Node nodesPool[2 * maxLen + 1];
Node* newNode(int left, int right, int depth, Node *parent = nullptr)
{
  return nodesPool[nodesCount++].init(left, right, depth, parent);
}

Node* root;

void buildSuffixTree()
{
  root = newNode(0, 0, 0);
  Node* node = root;
  int tail = -1;
  for (int i = 0; i < n; i++)
    {
      tail++;
      Node* last = nullptr;
      while (tail >= 0)
        {
          Node* child = node->children[s[i - tail] - 'a'];

          while (child != nullptr && child->right - child->left <= tail)
            {
              tail -= child->right - child->left;
              node = child;
              child = child->children[s[i - tail] - 'a'];
            }
          if (child == nullptr)
            {
              node->children[s[i] - 'a'] = newNode(i, n, node->depth + node->right - node->left, node);
              if (last != nullptr)
                last->suffLink = node;
              last = nullptr;
            }
          else
            {
              char t = s[child->left + tail];
              if (t == s[i])
                {
                  if (last != nullptr)
                    last->suffLink = node;
                  break;
                }
              else
                {
                  int const depth = node->depth + node->right - node->left;
                  Node* splitNode = newNode(child->left, child->left + tail, depth, node);
                  splitNode->children[s[i] - 'a'] = newNode(i, n, child->depth + tail, splitNode);
                  splitNode->children[t - 'a'] = child;
                  child->left += tail;
                  child->depth += tail;
                  child->parent = splitNode;
                  node->children[s[i - tail] - 'a'] = splitNode;
                  if (last != nullptr)
                    last->suffLink = splitNode;
                  last = splitNode;
                }
            }
          if (node == root)
            tail--;
          else
            node = node->suffLink;
        }
    }
}

void printNodeInfo(const Node* node)
{
  printf("%d %d %d\n", node->parent->dfsId, node->left, node->right);
}


void detour(Node* curr)
{
  static int id = 0;
  if (curr->dfsId == -1)
    curr->dfsId = id++;
  if (curr != root)
    printNodeInfo(curr);

  int counter = 0;
  for (int i = alpha - 1; counter < alpha; i = (i + 1) % alpha, counter++)
    if (curr->children[i])
      detour(curr->children[i]);
}

int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  n = 0;
  nodesCount = 0;
  substrNum = 0;

  char c;
  while ((c = getchar() )!= EOF)
    {
      if (c == '$' || c == '\n' || c == '\r')
        break;
      s[n++] = c;
    }
  s[n++] = 'z' + 1;

  buildSuffixTree();

  printf("%d\n", nodesCount);
  detour(root);


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}