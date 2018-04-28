#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include "../Test/sole.cpp"
#include "DOpenMP.cpp"

int main(int argc, char * argv[]) {

    if (argc != 5) {
        std::cout << "incorrect input values " << argc << std::endl;
        for (int i = 0; i < argc; i++) {
          std::cout << i << " " << argv[i] << '\n';
        }
        return 1;
    }
    int num_threads = atoi(argv[1]);
    int N;
    int MSize;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::ifstream in(argv[2], std::ios::binary);
    in.read((char*)&MSize, sizeof(MSize));
    if (MSize < 1) {
        std::cout << "error" << std::endl;
        in.close();
        return 3;
    }
    int tmp;
    int * matrix = new int [MSize * MSize];
    for(int i = 0; i < (MSize*MSize); i++){
        in.read((char*)&matrix[i], sizeof(matrix[i]));
    }
    int * res = new int [MSize];

    bool d_success;
    double time = 0;
    for (int i = 0; i < 10; i++) {
      start = std::chrono::system_clock::now();
      d_success = Dijkstra(matrix,0 ,MSize, res);
      end = std::chrono::system_clock::now();
      time += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    }
    time /= 10;

    std::ofstream ofs(argv[3], std::ios::binary);
    ofs.write((char*)&d_success, sizeof(d_success));
    ofs.write((char*)&time, sizeof(time));
    ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        ofs.write((char*)&res[i], sizeof(res[i]));
    }
    ofs.flush();
    ofs.close();

    double p_time = 0;
    std::chrono::time_point<std::chrono::system_clock> p_start;
    std::chrono::time_point<std::chrono::system_clock> p_end;
    for (int i = 0; i < 10; i++) {
      p_start = std::chrono::system_clock::now();
      //bool p_success = ompDijkstra(matrix,0 ,MSize, res);
      ompDijkstra(matrix,0 ,MSize, res, num_threads);
      p_end = std::chrono::system_clock::now();
      p_time += std::chrono::duration_cast<std::chrono::microseconds>(p_end-p_start).count();
    }
    p_time /= 10;

    std::ofstream p_ofs(argv[4], std::ios::binary);
    p_ofs.write((char*)&d_success, sizeof(d_success));
    p_ofs.write((char*)&time, sizeof(time));
    p_ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        p_ofs.write((char*)&res[i], sizeof(res[i]));
    }
    p_ofs.flush();
    p_ofs.close();
    std::cout <<"time is " << time << '\n' << "p_time is " << p_time << '\n';
//    for (int i = 0; i < MSize; i++) {
//      delete[] matrix[i];
//    }
    delete[] matrix;
    delete[] res;
}
