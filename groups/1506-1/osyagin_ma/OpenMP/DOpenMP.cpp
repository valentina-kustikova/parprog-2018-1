#include <iostream>
#include <climits>
#include <vector>
#include <omp.h>

class Package{
private:
  int index;
  int val;

public:
  Package(int new_val,int new_index){
    index = new_index;
    val = new_val;
  }

  Package(){
    index = INT_MAX;
    val = INT_MAX;
  }

  int getIndex() const {
    return index;
  }

  int getVal() const {
    return val;
  }

  void setIndex(int new_index){
    index = new_index;
  }

  void setVal(int new_val){
    val = new_val;
  }

  void devour(const Package & other){
    if(val > other.getVal()){
      val = other.getVal();
      index = other.getIndex();
    }
  }
};

bool ompDijkstra(int* GR, int st, int n, int* res, int num_thread)
{
    Package result(INT_MAX,INT_MAX);
    bool success = true;
    int* distance = new int[n];
    int count = 0;
    int index = 0;
    int u = 0;
    bool* visited = new bool[n];

    #pragma omp declare reduction(PackReduction:Package: \
    omp_out.devour(omp_in))

    for (int i = 0; i < n; i++) {
        distance[i] = INT_MAX;
        visited[i] = false;
    }
    distance[st] = 0;

    for (count = 0; count < n-1; count++) {
        result.setVal(INT_MAX);

        #pragma omp parallel for shared(visited,distance) reduction(PackReduction:result) num_threads (num_thread)
        for (int i = 0; i < n; i++) {
          if (!visited[i] && distance[i] <= result.getVal()) {
              result.setVal(distance[i]);
              result.setIndex(i);
          }
        }
        u = result.getIndex();
        visited[u] = true;

        #pragma omp parallel num_threads (num_thread)
        {
          #pragma omp for nowait
          for (int i = 0; i < n; i++)
              if (!visited[i] && GR[u * n + i] && distance[u] != INT_MAX && distance[u] + GR[u * n + i] < distance[i])
                  distance[i] = distance[u] + GR[u * n + i];
        }
    }
    for (int i = 0; i < n; i++) {
        res[i] = distance[i];
    }
    for (int i = 0; i < n; i++) {
        if (distance[i] == INT_MAX){
            success = false;
        }
    }
    delete[] visited;
    delete[] distance;
 return success;
}
