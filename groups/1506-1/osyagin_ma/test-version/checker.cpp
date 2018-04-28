#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
// Используется для взаимодействия с тестирующей системой
////////////////////////////////////////////////////////////////////////////////////////////

class result
{
	private:
		ofstream bur;
	public:

		result ( string str){ 
				bur.open(str); 
		}

		~result() { bur.close(); }

		void write_message(string str){  
			int l = str.size (); 
			bur << str; 
		}
		void write_time(double x){ 
			bur << x;  
		}
		void write_memory(unsigned long long x){ 
			bur << x; 
		}
};
////////////////////////////////////////////////////////////////////////////////////////////


int main (int argc, char* argv[]){
	char * name1;
    char * name2;
    char * name3;
    if (argc != 4){
        name1 = "tests/1perfect.ans";
        name2 = "tests/1.out";
        name3 = "result1.txt";
    }else{
        name1 = argv[1];
        name2 = argv[2];
        name3 = argv[3];
    }
    cout << name1 << " " << name2<< " " << name3 <<endl;
    result checker_result(name3); 
	ifstream in1(name1, std::ios::binary); //perfect
	ifstream in2(name2, std::ios::binary); //result 	
	bool success1,success2;
	int AC = -1;
    double time1, time2;
    int MSize1, MSize2;
    int VNum1, VNum2;
    in1.read((char*)&success1, sizeof(success1));
    in2.read((char*)&success2, sizeof(success2));

    cout << success1 << " " << success2 << " " << endl;

    if(success1 != success2){ 
    	checker_result.write_message("WA. Output is not correct.");
    }else{
    	if((success1 == success2) && (success1 == 0)){
    	    // graph is not connected
    		checker_result.write_message("AC. Both graphs is not connected.");
    	}else{
    		in2.read((char*)&time2, sizeof(time2));
    		in1.read((char*)&time1, sizeof(time1));
    		in1.read((char*)&MSize1, sizeof(MSize1));
    		in2.read((char*)&MSize2, sizeof(MSize2));
    		if(MSize1 == MSize2){
    			for (int i = 0; i < MSize1; i++) {
      				in1.read((char*)&VNum1, sizeof(VNum1));
      				in2.read((char*)&VNum2, sizeof(VNum2));
      				if(VNum1 == VNum2){
      					AC = 0;
      				}
    			}
    		}else{
    			checker_result.write_message("WA. Output is not correct.");
    		}
		}
	}

	// Проверяем, что ошибка мала,тогда сообщаем, что решение корректно, иначе - некорректно.
	if (AC == 0){
		checker_result.write_message("AC. Numbers are equal.");
	}else{
		checker_result.write_message("WA. Output is not correct.");
	}

	checker_result.write_time(time2);
	in1.close();
	in2.close();
	return 0;
}