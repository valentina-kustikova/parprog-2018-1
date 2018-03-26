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
        std::cout << "������������ ������" << std::endl;
        return 1;
    }
    int num_threads = atoi(argv[1]);//���������� �������
    int N;//������ �������
    int MSize;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::ifstream in(argv[2], std::ios::binary);//�������� ����� ��� ������
    in.read((char*)&N, sizeof(N));
    std::cout << N << '\n';
    if (N < 1) {
        std::cout << "�� ������������� ������ �������" << std::endl;
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
    std::cout << matr.size() << '\n';
    for(int e : matr){
    std::cout << e << " ";
    }
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
    std::cout << std::endl;
    for(int i = 0; i < MSize; i++){
        for(int j = 0; j <MSize; j++){
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    int * res = new int [MSize];
    start = std::chrono::system_clock::now();
    bool d_success = Dijkstra(matrix,0 ,MSize, res);
    end = std::chrono::system_clock::now();
    for(int i = 0; i < MSize; i++){
        std::cout << res[i] << " ";
    }

    double time = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();

    std::ofstream ofs(argv[3], std::ios::binary);
    //ofs << time;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << std::endl;
    //ofs.write((char*)&N, sizeof(N));
    ofs.write((char*)&d_success, sizeof(d_success));
    ofs.write((char*)&time, sizeof(time));
    ofs.write((char*)&MSize, sizeof(MSize));
    // ofs.write((char*)&MSize, sizeof(MSize));
    //ofs << d_success << MSize;
    for(int i = 0; i < MSize; i++){
        ofs.write((char*)&res[i], sizeof(res[i]));
       //ofs << i << res[i];
    }
    ofs.flush();
    ofs.close();
}
