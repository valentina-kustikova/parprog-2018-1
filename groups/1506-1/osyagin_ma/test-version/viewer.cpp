#include <iostream>
#include <vector>
#include <fstream>
//���������� �������� ����
//argv[1] - ��� �������� �����
int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect arguments" << std::endl;
        return 1;
    }

    std::ifstream solution(argv[1], std::ios::binary);
    bool success;
    double time;
    int MSize;
    int VNum,Route;
    solution.read((char*)&success, sizeof(success));
    std::cout << success << std::endl;
    solution.read((char*)&time, sizeof(time));
    std::cout << time << std::endl;
    solution.read((char*)&MSize, sizeof(MSize));
    std::cout << MSize << std::endl;
    for (int i = 0; i < MSize; i++) {
      solution.read((char*)&VNum, sizeof(VNum));
      std::cout << VNum << " ";
    }
    std::cout << std::endl;
}