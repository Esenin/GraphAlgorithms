//! @file
//! @author Ivan Senin
//! @brief Problem ZR. "Dictionary", Aho-Korasik
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
  const char *inName = "dictionary.in";
  const char *outName = "dictionary.out";
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


int const maxLen = 1000 * 1000 + 2;
int const maxM = 100000;
int const alpha = 26;
char s[maxLen];
char word[32];
int m, n, wordlen, nodeCount;


struct Node
{
  int parent;
  char prevChar;
  int sufLink, goodSL;
  array<int, alpha> children, transitions;
  bool leaf;
  vector<int> querieIds;

  void init()
  {
    sufLink = -1;
    goodSL = -1;
    fill(children.begin(), children.end(), -1);
    fill(transitions.begin(), transitions.end(), -1);
  }
};

array<Node, maxLen> nodes;
array<bool, maxM> queriesDone {};

void addString(int curQuery)
{
  int cur = 0;
  for (int i = 0; i < wordlen; i++)
    {
      int c = word[i] - 'a';
      if (nodes[cur].children[c] == -1)
        {
          nodes[nodeCount].init();
          nodes[nodeCount].parent = cur;
          nodes[nodeCount].prevChar = word[i];
          nodes[cur].children[c] = nodeCount++;
        }
      cur = nodes[cur].children[c];
    }
  nodes[cur].leaf = true;
  nodes[cur].querieIds.push_back(curQuery);
}


int transition(int, char); //forward
int findLink(int nodeIndex)
{
    Node& node = nodes[nodeIndex];
    if (node.sufLink == -1)
      node.sufLink = (node.parent == 0)? 0 : transition(findLink(node.parent), node.prevChar);
    return node.sufLink;
}

int transition(int nodeIndex, char ch)
{
    int c = ch - 'a';
    Node& node = nodes[nodeIndex];
    if (node.transitions[c] != -1)
      return node.transitions[c];

    if (node.children[c] != -1)
      return node.transitions[c] = node.children[c];

    return node.transitions[c] = (nodeIndex == 0)? 0 : transition(findLink(nodeIndex), ch);
}



void check(int nodeId)
{
  function<int(int)> getGoodSLink = [&] (int v)
  {
    if (nodes[v].goodSL == -1)
      {
        int u = findLink(v);
        if (u == 0)
          nodes[v].goodSL = 0;
        else
          nodes[v].goodSL = (nodes[u].leaf)? u : getGoodSLink(u);
      }
    return nodes[v].goodSL;
  };

  for(int u = nodeId; u != 0; u = getGoodSLink(u))
    {
      if (nodes[u].leaf)
        for(int x : nodes[u].querieIds)
          queriesDone[x] = true;
    }
}


int main()
{
#ifdef HOMERUN
  auto startTime = std::chrono::high_resolution_clock::now();
#endif
  ioInit();

  nodes[0].init();
  nodes[0].sufLink = 0;
  nodes[0].parent = -1;
  nodeCount = 1;

  scanf("%s", s);
  n = strlen(s);
  if (s[n - 1] == '\r' || s[n - 1] == '\n')
    n--;

  scanf("%d", &m);
  for (int i = 0; i < m; i++)
    {
      scanf("%s", word);
      wordlen = strlen(word);
      addString(i);
    }

  int node = 0;
  for (int i = 0; i < n; i++)
    {
      node = transition(node, s[i]);
      check(node);
    }

  for (int i = 0; i < m; i++)
    printf((queriesDone[i] ? "Yes\n" : "No\n"));


  ioClose();
#ifdef HOMERUN
  auto stopTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
  fprintf(stderr, "time elapsed [us] = %ld\n", duration.count());
#endif
  return 0;
}