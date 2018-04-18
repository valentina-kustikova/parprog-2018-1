#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include "sole.cpp"

int main(int argc, char * argv[]) {

    char * name1;
    char * name2;
    if (argc != 3){
        name1 = "tests/1.ans";
        name2 = "tests/1.out";
    }else{
        name1 = argv[1];
        name2 = argv[2];
    }

    int N;
    int MSize;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::ifstream in(name1, std::ios::binary);
    in.read((char*)&MSize, sizeof(MSize));
    if (MSize < 1) {
        in.close();
        return 3;
    }
    int tmp;
    int * matrix = new int [MSize * MSize];
    for(int i = 0; i < (MSize*MSize); i++){
        in.read((char*)&matrix[i], sizeof(matrix[i]));
    }
    int * res = new int [MSize];
    start = std::chrono::system_clock::now();
    bool d_success = Dijkstra(matrix,0 ,MSize, res);
    end = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::ofstream ofs(name2, std::ios::binary);
    ofs.write((char*)&d_success, sizeof(d_success));
    ofs.write((char*)&time, sizeof(time));
    ofs.write((char*)&MSize, sizeof(MSize));
    for(int i = 0; i < MSize; i++){
        ofs.write((char*)&res[i], sizeof(res[i]));
    }
    ofs.flush();
    ofs.close();
    delete [] res;
    delete [] matrix;
}
