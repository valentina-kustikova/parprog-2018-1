#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include "sole.cpp"

int main(int argc, char * argv[]) {

    if (argc != 4) {
        std::cout << "incorrect input values" << std::endl;
        return 1;
    }
    int num_threads = atoi(argv[1]);
    int N;
    int MSize;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::ifstream in(argv[2], std::ios::binary);
    in.read((char*)&N, sizeof(N));
    if (N < 1) {
        in.close();
        return 3;
    }
    std::vector<int> matr;
    int tmp;
    for(int j = 0;j < N;j++){
        in.read((char*)&tmp, sizeof(tmp));
        matr.push_back(tmp);
    }
    MSize = (int)sqrt(N);
    int** matrix = new int * [MSize];
    for(int i = 0; i < MSize; i++){
        matrix[i] = new int [MSize];
    }
    int vecI;
    for(int i = 0; i < MSize; i++){
        for(int j = 0; j <MSize; j++){
            vecI = i*MSize + j;
            matrix[i][j] = matr[vecI];
        }
    }
    int * res = new int [MSize];
    start = std::chrono::system_clock::now();
    bool d_success = Dijkstra(matrix,0 ,MSize, res);
    end = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    std::ofstream ofs(argv[3], std::ios::binary);
    ofs.write((char*)&d_success, sizeof(d_success));
    ofs.write((char*)&time, sizeof(time));
    ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        ofs.write((char*)&res[i], sizeof(res[i]));
    }
    ofs.flush();
    ofs.close();
}
