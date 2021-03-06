#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <queue>

using namespace std;

vector<int> parent_table, heights, group, graph[50000];
vector<bool> visited;
int brand_a_count, brand_b_count;

struct cmp {
  bool operator()(pair<long long, long long> p1, pair<long long, long long> p2) {
    if(p1.first == p2.first)
      return p1.second > p2.second;
    else
      return p1.first > p2.first;
  }
};

int find(int x) {
  if(parent_table[x] == x)
    return x;
  else
    return parent_table[x] = find(parent_table[x]);
}

void merge(int source, int target) {
  int root_s = find(source);
  int root_t = find(target);
  if(heights[root_s] < heights[root_t])
    parent_table[root_s] = root_t;
  else if(heights[root_s] > heights[root_t])
    parent_table[root_t] = root_s;
  else {
    parent_table[root_s] = root_t;
    heights[root_t]++;
  }
}

void make_max_brand_count_in_group(int node_count, int group_size, int x, int brand) {
  if(node_count == group_size)
    return;
  // cout << "brand : " << brand << endl;

  if(brand == 1) {
    brand_a_count++;
    brand = 2;
  }
  else {
    brand_b_count++;
    brand = 1;
  }

  visited[x] = true;
  for(int i = 0; i < group_size; i++) {
    if(graph[x][group[i]] && !visited[group[i]]) {
      make_max_brand_count_in_group(node_count+1, group_size, group[i], brand);
    }
  }
}


int main() {
  int n, i, j;
  scanf("%d", &n);
  vector<pair<long long, long long> > buildings;
  priority_queue<pair<long long, long long>, vector<pair<long long, long long> >, cmp> pq;
  for(i = 0; i < n; i++) {
    long long x, y;
    scanf("%lld %lld", &x, &y);
    pq.push(make_pair(x, y));
  }

  for(i = 0; i < n; i++) {
    buildings.push_back(pq.top());
    pq.pop();
  }

  heights.assign(n, 0);
  parent_table.assign(n, 0);
  for(i = 0; i < n; i++) {
    parent_table[i] = i;
  }

  for(i = 0; i < n; i++) {
    graph[i].assign(n, 0);
  }

  // for(i = 0; i < n; i++) {
  //   cout << "i : " << i << ", " << buildings[i].first << ", " << buildings[i].second << endl;
  // }



  for(i = 0; i < n-1; i++) {
    int count = 2;
    for(j = i+1; j < n && count != 0; j++) {
      if(buildings[j].first - buildings[i].first > 5)
        break;

      if(abs(buildings[j].second - buildings[i].second) > 5)
        continue;
      // if(buildings[j].first - buildings[i].first + abs(buildings[j].second - buildings[i].second) <= 7) {
      //   merge(j, i);
      //   graph[i][j] = 1;
      //   graph[j][i] = 1;
      //   count--;
      // }
      // else if(sqrt(pow(buildings[i].first - buildings[j].first, 2) + pow(buildings[i].second - buildings[j].second, 2)) <= 5) {
      if(sqrt(pow(buildings[i].first - buildings[j].first, 2) + pow(buildings[i].second - buildings[j].second, 2)) <= 5) {
        merge(j, i);
        graph[i][j] = 1;
        graph[j][i] = 1;
        count--;
      }
    }
  }

  // for(i = 0; i < n; i++) {
  //   for(j = 0; j < n; j++) {
  //     cout << graph[i][j] << ", ";
  //   }
  //   cout << endl;
  // }

  map<int, vector<int> > no_together_group;
  for(i = 0; i < n; i++) {
    no_together_group[find(i)].push_back(i);
  }

  int total_max_brand_count = 0;
  for(auto it = no_together_group.begin(); it != no_together_group.end(); it++) {
    visited.assign(n, false);
    group = it->second;
    brand_a_count = 0;
    brand_b_count = 0;
    int group_size = group.size();

    // cout << "group : " << it->first << endl;
    // for(auto x : it->second) {
    //   cout << x << ", ";
    // }
    // cout << endl;

    make_max_brand_count_in_group(0, group_size, group[0], 1);

    if(brand_a_count > brand_b_count)
      total_max_brand_count += brand_a_count;
    else
      total_max_brand_count += brand_b_count;

    // cout << "brand_a_count : " << brand_a_count << ", brand_b_count : " << brand_b_count << endl;
  }
  cout << n - total_max_brand_count << endl;

  return 0;
}
