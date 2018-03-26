#include <iostream>
#include <climits>
#include <vector>

bool Dijkstra(int** GR, int st, int n, int* res)
{
    bool success = true;
    int distance[n], count, index, i, u, m = st + 1;
    std::vector <int> result;
    bool visited[n];
    for (i = 0; i < n; i++)
    {
        distance[i] = INT_MAX; visited[i] = false;
    }
    distance[st] = 0;
    for (count = 0; count < n-1; count++)
    {
        int min = INT_MAX;
        for (i = 0; i < n; i++)
            if (!visited[i] && distance[i] <= min)
            {
                min = distance[i];
                index = i;
            }
        u = index;
        visited[u] = true;
        for (i = 0; i < n; i++)
            if (!visited[i] && GR[u][i] && distance[u] != INT_MAX && distance[u] + GR[u][i] < distance[i])
                distance[i] = distance[u] + GR[u][i];
    }
    for (i = 0; i < n; i++){
        res[i] = distance[i];
    }
    for (i = 0; i < n; i++) {
        if (distance[i] != INT_MAX)
            std::cout << m << " > " << i + 1 << " = " << distance[i] << std::endl;
        else{
              std::cout << m << " > " << i + 1 << " = " << "маршрут недоступен" << std::endl;
              success = false;

        }


    }
 return success;
}
