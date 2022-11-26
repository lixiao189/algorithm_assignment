#include <iostream>
#include <vector>

using std::vector;

void bubble_sort(vector<int> &arr) {
  for (int i = 0; i < arr.size(); ++i) {
    for (int j = 0; j < arr.size() - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

int main() {
  int n;
  std::cin >> n;
  vector<int> arr(n, 0);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  bubble_sort(arr);

  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;
}