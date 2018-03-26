#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char * argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc != 3) {
        std::cout << "Некорректные данные" << std::endl;
        return 1;
    }
    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "Файл для чтения не может быть открыт" << std::endl;
        return 2;
    }
    std::vector<int> v; //matrix buffer
    int tmp;
    while(!in.eof()){ //read matrix
        in >> tmp;
        std::cout << tmp << " ";
        v.push_back(tmp);
    }
    int N;
    N = v.size() - 1;
    if (N < 1) {
        std::cout << "Некорректный размер системы" << std::endl;
        return 3;
    }
    std::ofstream ofs(argv[2], std::ios::binary);
    ofs.write((char*)&N, sizeof(N));
    for(int elem:v){
        ofs.write((char*)&elem, sizeof(elem));
    }
    in.close();
    ofs.close();
    return 0;
}
