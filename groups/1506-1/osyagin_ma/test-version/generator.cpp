#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <climits>
#include <string>

void perfect(int* matr,int n, int st, std::string str) {
    int distance[n], count, index, i, u, m = st + 1;
    bool visited[n];
    for (i = 0; i < n; i++) {
        distance[i] = INT_MAX;
        visited[i] = false;
    }
    distance[st] = 0;
    for (count = 0; count < n - 1; count++) {
        int min = INT_MAX;
        for (i = 0; i < n; i++)
            if (!visited[i] && distance[i] <= min) {
                min = distance[i];
                index = i;
            }
        u = index;
        visited[u] = true;
        for (i = 0; i < n; i++)
            if (!visited[i] && matr[u*n + i] && distance[u] != INT_MAX &&
                distance[u] + matr[u*n + i] < distance[i])
                distance[i] = distance[u] + matr[u*n + i];
    }
    bool flag;
    for (i = 0; i < n; i++) {
        if (distance[i] != INT_MAX){
        	flag = true;
        }else{
        	if (m != 1){
        		flag = false;
        		break;
        	}
        }
    }
  double time = 0;
  std::ofstream ofs(str, std::ios::binary);
    ofs.write((char*)&flag, sizeof(flag));
    ofs.write((char*)&time, sizeof(time));
    ofs.write((char*)&n, sizeof(n));
    for(int i = 0; i < n; i++){
        ofs.write((char*)&distance[i], sizeof(distance[i]));
    }
}

int main (int argc, char* argv[]){
    char * name1;
    char * name2;
    if (argc != 3){
        name1 = "1000";
        name2 = "1";
    }else{
        name1 = argv[1];
        name2 = argv[2];
    }
	int n = atoi(name1);
	//generation int from 0 to 9
	std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution <int> distribution(0,99);
	distribution(engine);
	// create matrix
	int * inArr = new int [n * n];
	//fill the matrix with random numbers
	for (int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i != j){
				inArr[i * n + j] = distribution(engine);
				inArr[j * n + i] = inArr[i * n + j];
			}
		}
	}
	//write input to a file
	std::string str_in = name2;
	str_in += ".ans";
 	std::ofstream f(str_in, std::ios::binary);
    if (f.fail()) {
   		std::cout << "\n Ошибка открытия файла";
    }
    f.write((char*)&n, sizeof(n));
  	for(int i = 0; i < n; i++)
   	{
    	for(int j = 0; j < n; j++){
            f.write((char*)&inArr[i * n + j], sizeof(inArr[i * n + j]));
        }
   	}
	f.close();
	std::string str_out = argv[2];
	str_out += "perfect.ans";
	perfect(inArr, n, 0, str_out);
    delete [] inArr;
	return 0;
}
