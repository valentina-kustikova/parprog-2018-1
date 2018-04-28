#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char * argv[]) {
    char * name1;
    if (argc != 2){
        name1 = "1.ans";
    }else{
        name1 = argv[1];
    }
    int MSize;
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
    for(int i = 0; i < MSize; i++){
        for(int j = 0; j < MSize; j++){
            std::cout << matrix[i * MSize + j] << " ";
        }
        std::cout<< std::endl;
    }
    in.close();
    return 0;
}
