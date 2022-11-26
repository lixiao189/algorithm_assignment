#include <algorithm>
#include <iostream>

const int N = 1e5 + 5;

struct Object {
  int p;
  int w;
};

bool cmp_object(Object a, Object b) {
  return (double)a.p / (double)a.w > (double)b.p / (double)b.w;
}

int neighborhood_search(int n, int m, Object *objects, int *W) {
  std::sort(objects, objects + n, cmp_object);

  bool x[N];
  int W_used[N];
  memset(x, 0, sizeof(x));
  memset(W_used, 0, sizeof(W_used));
  for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
      if (!x[i] && W[j] - W_used[j] >= objects[i].w) {
        x[i] = true;
        W_used[j] += objects[i].w;
      }
    }
  }

  int result = 0;
  for (int i = 0; i < n; i++) {
    if (x[i]) {
      result += objects[i].p;
    }
  }

  return result;
}

int main() {
  int n, m;
  Object objects[N];
  int W[N];

  // Input user data
  std::cin >> n >> m;
  for (int i = 0; i < n; i++) {
    std::cin >> objects[i].p >> objects[i].w;
  }
  for (int i = 0; i < m; i++) {
    std::cin >> W[i];
  }

  int result = neighborhood_search(n, m, objects, W);
  std::cout << result << std::endl;

  return 0;
}
