//! @file
//! @author Ivan Senin
//! @brief Problem ZD. Nim

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
  freopen("pawns.in", "r", stdin);
  freopen("pawns.out", "w", stdout);
}

void postActions()
{
  fclose(stdin);
  fclose(stdout);
}

uint Inf = numeric_limits<uint>::max();
size_t n = 0;
size_t m = 0;

vector<uint> cache;
vector<uint> used;

uint version = 0;
uint primary = 55;
uint secondary = 34;


uint mex(vector<uint> const &in)
{
  version++;
  uint const count = in.size();
  for (uint i = 0; i < count; i++)
    if (in[i] <= primary + secondary)
      used[in[i]] = version;

  for (uint i = 0; i < count ; ++i)
    if (used[i] != version)
      return i;
  return count;
}

uint grundy(uint const nn)
{
  if (nn == 0)
    return 0;
  else if (nn == 1)
    return 1;

  vector<uint> inbuf;
  for (uint i = 2u; i < nn; i++)
    {
      inbuf.push_back(cache[i - 2] ^ cache[nn - i - 1]);
    }
  inbuf.push_back(cache[nn - 2]);

  uint const result = mex(inbuf);
  cache[nn] = result;
  return result;
}



int main()
{
  initSolution();
  scanf("%d", &n);

  uint const len = primary + secondary;

  cache.resize(2* len + 1);
  cache[1] = 1;
  used.resize(2* len + 1);

  version = 0;

  uint result = 0;

  for (uint i = 0u; i <= len; i++)
    grundy(i);

  if (n <= len)
    result = cache[n];
  else
    {
      uint cycled = (n - primary) % secondary;
      cycled = (cycled)? cycled : secondary;
      result = cache[primary + cycled];
    }

  if (result)
    printf("White\n");
  else
    printf("Black\n");

  postActions();
  return 0;
}