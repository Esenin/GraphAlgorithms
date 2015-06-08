//! @file
//! @author Ivan Senin
//! @brief Problem ZA. Retro-analysis

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>


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
  freopen("tickets.in", "r", stdin);
  freopen("tickets.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint Inf = numeric_limits<uint>::max();
size_t n = 0;
size_t m = 0;

vector<uint> coins;
vector<bool> winpos;


bool canJump2False(int const pos)
{
  for(auto &x : coins)
    {
      if (!winpos[pos + x])
        return true;
    }
  return false;
}


int main()
{
  initSolution();
  scanf("%d %d", &n, &m);

  coins.reserve(m);
  uint max = 0;
  for (uint i = 0; i < m; i++)
    {
      uint x;
      scanf("%d", &x);
      coins.push_back(x);
      if (x > max)
        max = x;
    }

  winpos.resize(n + max, false);

  for (int i = n - 1; i >= 0; i--)
    {
      winpos[i] = canJump2False(i);
    }

  if (winpos[0])
    {
      printf("PETYA\n");
    }
  else
    {
      printf("MISHA\n");
    }

  postActions();
  return 0;
}