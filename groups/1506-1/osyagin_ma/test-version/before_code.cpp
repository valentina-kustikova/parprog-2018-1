#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include "sole.cpp"
#include "../OpenMP/DOpenMP.cpp"
#include "../TBB/DTBB.cpp"


int main(int argc, char * argv[]) {

    if (argc != 6) {
        std::cout << "incorrect input values " << argc << std::endl;//}
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
        std::cout << "task size error" << std::endl;
        in.close();
        return 3;
    }
    std::cout << "task size is: " << MSize << std::endl;
    int tmp;
    int * matrix = new int [MSize * MSize];
    for(int i = 0; i < (MSize*MSize); i++){
        in.read((char*)&matrix[i], sizeof(matrix[i]));
    }
    int * res = new int [MSize];

    bool d_success, mp_suc, tbb_suc;
    double time = 0;

    //sequal version
    start = std::chrono::system_clock::now();
    d_success = Dijkstra(matrix,0 ,MSize, res);
    end = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    std::cout << "sequential time is: " << (int)time << std::endl;

    std::ofstream ofs(argv[3], std::ios::binary);
    ofs.write((char*)&d_success, sizeof(d_success));
    ofs.write((char*)&time, sizeof(time));
    ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        ofs.write((char*)&res[i], sizeof(res[i]));
    }
    ofs.flush();
    ofs.close();

    double omp_time = 0;
    std::chrono::time_point<std::chrono::system_clock> omp_start;
    std::chrono::time_point<std::chrono::system_clock> omp_end;
    //OpenMP version
    omp_start = std::chrono::system_clock::now();
    mp_suc = ompDijkstra(matrix,0 ,MSize, res, num_threads);
    omp_end = std::chrono::system_clock::now();
    omp_time = std::chrono::duration_cast<std::chrono::microseconds>(omp_end-omp_start).count();
    std::cout << "omp time is: " << (int)omp_time << std::endl;

    std::ofstream omp_ofs(argv[4], std::ios::binary);
    omp_ofs.write((char*)&d_success, sizeof(d_success));
    omp_ofs.write((char*)&time, sizeof(time));
    omp_ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        omp_ofs.write((char*)&res[i], sizeof(res[i]));
    }
    omp_ofs.flush();
    omp_ofs.close();

    double tbb_time = 0;
    std::chrono::time_point<std::chrono::system_clock> tbb_start;
    std::chrono::time_point<std::chrono::system_clock> tbb_end;
    //TBB version
    tbb_start = std::chrono::system_clock::now();
    tbb_suc = tbbDijkstra(matrix,0 ,MSize, res, num_threads);
    tbb_end = std::chrono::system_clock::now();
    tbb_time = std::chrono::duration_cast<std::chrono::microseconds>(tbb_end-tbb_start).count();
    std::cout << "tbb time is: " << (int)tbb_time << std::endl;

    std::ofstream tbb_ofs(argv[5], std::ios::binary);
    tbb_ofs.write((char*)&d_success, sizeof(d_success));
    tbb_ofs.write((char*)&time, sizeof(time));
    tbb_ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        tbb_ofs.write((char*)&res[i], sizeof(res[i]));
    }
    tbb_ofs.flush();
    tbb_ofs.close();
    std::cout << std::endl;
    delete[] matrix;
    delete[] res;
}
