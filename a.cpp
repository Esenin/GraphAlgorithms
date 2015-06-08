//! @author Ivan Senin

#include <fstream>
#include <string.h>

using namespace std;

int main()
{
  ios_base::sync_with_stdio(false);

  string const fileName("sortuniq");

  fstream inFile;
  inFile.open(fileName + ".in", ios_base::in );
  fstream outFile;
  outFile.open(fileName + ".out", ios_base::out | ios_base::trunc);

  size_t size = 0;
  inFile >> size;

  for (size_t i = 0; i < size; i++)
    {
      outFile << (i + 1) << " ";
    }

  inFile.close();
  outFile.close();

  return 0;
}