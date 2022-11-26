#include <iostream>
#include <vector>

using std::vector;

void merge_sort(vector<int> &arr) {
  if (arr.size() <= 1) {
    return;
  }

  int mid = arr.size() / 2;
  vector<int> left(arr.begin(), arr.begin() + mid);
  vector<int> right(arr.begin() + mid, arr.end());

  merge_sort(left);
  merge_sort(right);

  int i = 0, j = 0, k = 0;
  while (i < left.size() && j < right.size()) {
    if (left[i] < right[j]) {
      arr[k++] = left[i++];
    } else {
      arr[k++] = right[j++];
    }
  }

  while (i < left.size()) {
    arr[k++] = left[i++];
  }

  while (j < right.size()) {
    arr[k++] = right[j++];
  }
}

int main() {
  auto n = 0;
  std::cin >> n;
  vector<int> arr(n, 0);
  for (auto i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  merge_sort(arr);

  for (auto i = 0; i < n; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}