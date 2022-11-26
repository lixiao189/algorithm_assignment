#include <algorithm>
#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <unistd.h>
#include <vector>

using std::list;
using std::sort;
using std::vector;

const int N = 1e3 + 5;

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

int calculate_result(vector<Knapsack> &knapsacks) {
  int result = 0;

  for (auto &knapsack : knapsacks) {
    for (auto &object : knapsack.objects) {
      result += object.p;
    }
  }

  return result;
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

  return calculate_result(knapsacks);
}

int neighborhood_search(vector<Object> &objects, vector<Knapsack> &knapsacks,
                        vector<bool> &flag) {
  int global_result = greedy(objects, knapsacks, flag);
  int tmp_result = global_result;

  // Store objects outside
  list<Object> outside_object;
  for (int i = 0; i < objects.size(); i++) {
    if (!flag[objects[i].index])
      outside_object.push_back(objects[i]);
  }

  auto object_it = outside_object.begin();
  auto put_in = false;
  while (object_it != outside_object.end()) {
    put_in = false;
    for (auto i = 0; i < knapsacks.size() && object_it != outside_object.end();
         i++) {
      if (object_it->w <= (knapsacks[i].capacity - knapsacks[i].cost)) {
        // Check whether m1 has enough space
        tmp_result += object_it->p;
        put_in = true;
        knapsacks[i].objects.push_back(*object_it);
        knapsacks[i].cost += object_it->w;
        object_it = outside_object.erase(object_it);
        continue;
      }

      for (auto j = i + 1;
           j < knapsacks.size() && object_it != outside_object.end(); j++) {
        auto it1 = knapsacks[i].objects.begin();
        auto knapsack1_removed = false;
        while (it1 != knapsacks[i].objects.end() &&
               object_it != outside_object.end()) {
          knapsack1_removed = false;
          if (object_it->w <=
              knapsacks[i].capacity - knapsacks[i].cost + it1->w) {
            // Check whether m1 can make room for object k
            if (it1->w <= (knapsacks[j].capacity - knapsacks[j].cost)) {
              // Check whether m2 can hold object in m1
              tmp_result += object_it->p;
              put_in = true;
              knapsack1_removed = true;

              knapsacks[i].cost += object_it->w;
              knapsacks[i].cost -= it1->w;
              knapsacks[j].cost += it1->w;

              knapsacks[i].objects.push_back(*object_it);
              knapsacks[j].objects.push_back(*it1);

              it1 = knapsacks[i].objects.erase(it1);
              object_it = outside_object.erase(object_it);
              break;
            }

            auto it2 = knapsacks[j].objects.begin();
            auto knapsack2_removed = false;
            while (it2 != knapsacks[j].objects.end() &&
                   object_it != outside_object.end()) {
              knapsack2_removed = false;
              if (it1->w <=
                      (knapsacks[j].capacity - knapsacks[j].cost + it2->w) &&
                  object_it->p > it2->p) {
                // Check whether m2 can make room for object in m1
                tmp_result -= it2->p;
                tmp_result += object_it->p;
                knapsack1_removed = true;
                knapsack2_removed = true;
                put_in = true;

                knapsacks[i].cost -= it1->w;
                knapsacks[i].cost += object_it->w;
                knapsacks[j].cost -= it2->w;
                knapsacks[j].cost += it1->w;

                outside_object.push_back(*it2);
                knapsacks[i].objects.push_back(*object_it);
                knapsacks[j].objects.push_back(*it1);

                it1 = knapsacks[i].objects.erase(it1);
                it2 = knapsacks[j].objects.erase(it2);
                object_it = outside_object.erase(object_it);

                break;
              }

              if (!knapsack2_removed) {
                it2++;
              }
            }
          }

          if (!knapsack1_removed) {
            it1++;
          }
        }
      }
    }

    if (!put_in) {
      object_it++;
    }

    if (tmp_result >= global_result) {
      global_result = tmp_result;
    } else {
      return global_result;
    }
  }

  return global_result;
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
  std::cout << "-------------------result------------------" << std::endl;
  std::cout << result << std::endl;

  // detail data output
  output_knapsacks(knapsacks);

  return 0;
}
