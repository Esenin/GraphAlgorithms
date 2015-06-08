//! @author Ivan Senin

#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

fstream in;
fstream out;
size_t size = 0;

void initSolution()
{
  ios_base::sync_with_stdio(false);
  string const fileName("sort");

  in.open(fileName + ".in", ios_base::in);
  out.open(fileName + ".out", ios_base::out | ios_base::trunc);
}

void postActions()
{
  in.close();
  out.close();
}


int right(int &index)
{
  return 2 * index + 2;
}
int left(int &index)
{
  return 2 * index + 1;
}
int parent(int &index)
{
  return (index - 1) / 2;
}

void heapify(vector<int> &arr, int i, int rightBound)
{
  int l = left(i), r = right(i), largest = 0;
  if (l < rightBound && arr[l] > arr[i])
    largest = l;
  else
    largest = i;

  if (r < rightBound && arr[r] > arr[largest])
    largest = r;

  if (largest != i)
    {
      swap(arr[i], arr[largest]);
      heapify(arr, largest, rightBound);
    }
}

void buildHeap(vector<int> &arr)
{
  for (int i = arr.size() / 2; i >= 0; i--)
    heapify(arr, i, arr.size());
}

void heapSort(vector<int> &arr)
{
  buildHeap(arr);
  for (int i = arr.size() - 1; i >= 1; i--)
    {
      swap(arr[0], arr[i]);
      heapify(arr, 0, i);
    }
}

int main()
{
  initSolution();
  in >> size;


  vector<int> array(size);
  for (size_t i = 0; i < size; i++)
      in >> array[i];

  heapSort(array);

  for (size_t i = 0; i < size; i++)
    out << array[i] << " ";


  postActions();
  return 0;
}