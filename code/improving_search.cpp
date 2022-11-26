#include <algorithm>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <unistd.h>
#include <vector>

#define in(container, object)                                                  \
  (std::find(container.begin(), container.end(), object) != container.end())

using std::list;
using std::sort;
using std::vector;

const int N = 1e3 + 5;
const int INF = 0x3f3f3f3f;

struct Object {
  int p;
  int w;
  int index;
};

struct Knapsack {
  int capacity;
  int cost;
  int index;

  list<Object> objects;
};

bool operator==(const Object &a, const Object &b) {
  return a.p == b.p && a.w == b.w;
}

bool cmp_object(Object a, Object b) {
  return (double)a.p / (double)a.w > (double)b.p / (double)b.w;
}

void output_knapsacks(vector<Knapsack> &knapsacks) {
  // detail data output
  std::cout << "---------------knapsack info---------------" << std::endl;
  for (auto &knapsack : knapsacks) {
    std::cout << "Knapsack's index: " << knapsack.index << std::endl;
    std::cout << "Knapsack's capacity: " << knapsack.capacity << std::endl;
    std::cout << "Knapsack's cost: " << knapsack.cost << std::endl;
    for (auto &object : knapsack.objects) {
      std::cout << object.index << " " << object.p << " " << object.w
                << std::endl;
    }
  }
}

int calculate_result(vector<Object> &objects, vector<int> solution) {
  int result = 0;

  for (auto &object : objects) {
    if (solution[object.index] != -1) {
      result += object.p;
    }
  }

  return result;
}

int greedy(vector<Object> &objects, vector<Knapsack> &knapsacks,
           vector<bool> &flag) {
  sort(objects.begin(), objects.end(), cmp_object);

  for (auto &knapsack : knapsacks) {
    for (auto i = 0; i < objects.size(); i++) {
      if (!flag[objects[i].index] &&
          knapsack.capacity - knapsack.cost >= objects[i].w) {
        flag[objects[i].index] = true;
        knapsack.cost += objects[i].w;
        knapsack.objects.push_back(objects[i]);
      }
    }
  }

  int result = 0;

  for (auto &knapsack : knapsacks) {
    for (auto &object : knapsack.objects) {
      result += object.p;
    }
  }

  return result;
}

int neighborhood_search(vector<Object> &objects, vector<Knapsack> &knapsacks,
                        vector<bool> &flag) {
  auto result = greedy(objects, knapsacks, flag);

  // Using vector to store the solution
  vector<int> solution(objects.size(), -1); // -1 stands for outside
  for (auto &knapsack : knapsacks) {
    for (auto &object : knapsack.objects) {
      solution[object.index] = knapsack.index;
    }
  }

  while (true) {
    // Generate neighbors
    vector<vector<int>> neighbors;
    for (auto i = 0; i < objects.size(); i++) {
      if (solution[objects[i].index] == -1) {
        // put the object into knapsack
        for (auto &knapsack : knapsacks) {
          if (knapsack.capacity - knapsack.cost >= objects[i].w) {
            auto neighbor = solution;
            neighbor[objects[i].index] = knapsack.index;
            neighbors.push_back(neighbor);
          }
        }
        // Find appropriate object in knapsacks to throw out and put the object
        for (auto &knapsack : knapsacks) {
          for (auto &object : knapsack.objects) {
            if (object.index != objects[i].index && object.w >= objects[i].w) {
              auto neighbor = solution;
              neighbor[object.index] = -1;
              neighbor[objects[i].index] = knapsack.index;
              neighbors.push_back(neighbor);
            }
          }
        }
        // Rotate
        for (auto ki = 0; ki < knapsacks.size(); ki++) {
          for (auto &object1 : knapsacks[ki].objects) {
            for (auto kj = 0; kj < knapsacks.size(); kj++) {
              auto ki_rest = knapsacks[ki].capacity - knapsacks[ki].cost;
              auto kj_rest = knapsacks[kj].capacity - knapsacks[kj].cost;
              if (object1.w <= kj_rest && objects[i].w <= ki_rest + object1.w &&
                  kj != ki) {
                auto neighbor = solution;
                neighbor[objects[i].index] = knapsacks[ki].index;
                neighbor[object1.index] = knapsacks[kj].index;
                neighbors.push_back(neighbor);
              } else {
                for (auto &object2 : knapsacks[kj].objects) { // Throw object2
                  if (object1.w <= kj_rest + object2.w &&
                      objects[i].w <= ki_rest + object1.w &&
                      object2.p < objects[i].p && ki != kj) {
                    auto neighbor = solution;
                    neighbor[objects[i].index] = knapsacks[ki].index;
                    neighbor[object1.index] = knapsacks[kj].index;
                    neighbor[object2.index] = -1;
                    neighbors.push_back(neighbor);
                  }
                }
              }
            }
          }
        }
      }
    }

    // Find the best neighbor
    auto maxn_neighbor_result = -INF;
    for (auto &neighbor : neighbors) {
      int neighbor_result = calculate_result(objects, neighbor);

      if (neighbor_result > maxn_neighbor_result) {
        maxn_neighbor_result = neighbor_result;
        solution = neighbor;
      }
    }

    if (result < maxn_neighbor_result) {
      result = maxn_neighbor_result;
    } else {
      break;
    }

    // Update knapsacks
    for (auto &knapsack : knapsacks) {
      knapsack.objects.clear();
      knapsack.cost = 0;

      for (auto i = 0; i < objects.size(); i++) {
        if (solution[objects[i].index] == knapsack.index) {
          knapsack.objects.push_back(objects[i]);
          knapsack.cost += objects[i].w;
        }
      }

      if (knapsack.cost > knapsack.capacity) {
        std::cout << "Error!" << std::endl;
        output_knapsacks(knapsacks);
        return -1;
      }
    }
  }

  return result;
}

int main() {
  int n, m, p, w, W;
  vector<Object> objects;
  vector<Knapsack> knapsacks;

  // Input user data
  std::cin >> n >> m;
  for (int i = 0; i < n; i++) {
    std::cin >> p >> w;
    objects.push_back(Object{p, w, i});
  }
  for (int i = 0; i < m; i++) {
    std::cin >> W;
    knapsacks.push_back(Knapsack{W, 0, i});
  }

  vector<bool> flag(objects.size(), false);
  int result = neighborhood_search(objects, knapsacks, flag);
  std::cout << result << std::endl;

  return 0;
}
