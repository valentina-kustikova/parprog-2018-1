#include <iostream>
#include <climits>
#include <vector>
#include <tbb/tbb.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>

class MinSearcher {
private:
  int* distance;
  bool* visited;
  int min;
  int index;

public:
  explicit MinSearcher(int* new_distance,bool* new_visited):
    distance(new_distance),visited(new_visited),min(INT_MAX),index(0){}

  MinSearcher(const MinSearcher& ms, tbb::split):
    distance(ms.distance),visited(ms.visited),min(INT_MAX),index(0){}

  void join(const MinSearcher& ms){
    if(ms.min < min){
      min = ms.min;
      index = ms.index;
    }
  }

  int getMin() const {
    return min;
  }

  int getIndex() const {
    return index;
  }

  void operator()(const tbb::blocked_range<int>& r) {
    for (int i = r.begin(); i < r.end(); i++)
        if (!visited[i] && distance[i] <= min)
        {
            min = distance[i];
            index = i;
        }
  }
};

class VertexUpdater{
private:
  int* distance;
  bool* visited;
  int* GR;
  int u;
  int n;
public:
  VertexUpdater(int* new_distance,bool* new_visited,int* new_GR, int new_u,int new_n):
    distance(new_distance),visited(new_visited),GR(new_GR),u(new_u),n(new_n){}

  VertexUpdater(const VertexUpdater &vo):
    distance(vo.distance),visited(vo.visited),GR(vo.GR),u(vo.u),n(vo.n){}


  void operator()(tbb::blocked_range<int>& r) const {
    for (int i = r.begin(); i < r.end(); i++)
        if (!visited[i] && GR[u * n + i] && distance[u] != INT_MAX && distance[u] + GR[u * n + i] < distance[i])
            distance[i] = distance[u] + GR[u * n + i];
  }


};

bool tbbDijkstra(int* GR, int st, int n, int* res,int threads) {
    tbb::task_scheduler_init init(threads);
    bool success = true;
    int* distance = new int[n];
    int count, index, i, u, m = st + 1;
    bool* visited = new bool[n];
    for (i = 0; i < n; i++)
    {
        distance[i] = INT_MAX; visited[i] = false;
    }
    distance[st] = 0;
    for (count = 0; count < n-1; count++) {
        MinSearcher ms(distance,visited);
        tbb::parallel_reduce(tbb::blocked_range<int>(0,n,n/threads),ms);
        u = ms.getIndex();
        visited[u] = true;

        tbb::parallel_for(tbb::blocked_range<int>(0,n,n/threads),
          VertexUpdater(distance,visited,GR,u,n));
    }
    for (i = 0; i < n; i++){
        res[i] = distance[i];
    }
    for (i = 0; i < n; i++) {
        if (distance[i] == INT_MAX){
            success = false;
        }
    }
 return success;
}
