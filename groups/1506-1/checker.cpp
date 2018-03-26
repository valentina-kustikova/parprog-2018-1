#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;
// Используется для взаимодействия с тестирующей системой
////////////////////////////////////////////////////////////////////////////////////////////
/*
// Checker может устанавливать вот эти три вердикта:
AC = Accepted = Решение выдаёт корректный результат на данном тесте
WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте
PE = Presentation Error = Ошибка формата выходных данных
// Остальные вердикты checker не может устанавливать
NO = No verdict = Вердикт отсутствует
CE = Compilation Error = Ошибка компиляции
ML = Memory Limit Exceeded = Превышено ограничение по памяти
TL = Time Limit Exceeded = Превышено ограничение по времени работы
RE = Runtime Error = Ошибка времени исполнения программы
IL = Idle Limit Exceeded = Превышено время простоя (бездействия) программы
DE = Deadly Error = Ошибка тестирующей системы
*/
enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };
class result
{
	private:
		FILE * bur;
	public:
		enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };

		result (bool read = false){
			if (read) 
				bur = fopen("result.txt", "r");
			else 
				bur = fopen("result.txt", "w"); 
		}

		~result() { fclose (bur); }

		void write_type(ext_cls t) { fwrite(&t, sizeof (t), 1, bur); }
		// Сообщить тестирующей системе, что решение получило один из вердиктов verdict
		void write_verdict(verdict v){
			write_type(ext_cls::VERDICT); 
			fwrite(&v, sizeof (v), 1, bur); 
		}
		// Написать сообщение от checker'a пользователю.
		//Например, что решение верное, или неверное.
		//Использовать только латинские буквы и знаки препинания
		void write_message(string str){ 
			write_type(ext_cls::MESSAGE); 
			int l = str.size (); 
			fwrite(&l, sizeof (l), 1, bur);
			fwrite (&str[0], sizeof (str[0]), l, bur); 
		}
		// Сообщить тестирующей системе время работы программы участника,
		//вычисленное с помощью before_code
		// x имеет размерность 100 нс = 10 ^ (-7) сек
		void write_time(long long x){ 
			write_type(ext_cls::TIME); 
			fwrite(&x, sizeof (x), 1, bur); 
		}
		// Сообщить тестирующей системе, память затребованную программой участника
		void write_memory(unsigned long long x){ 
			write_type(ext_cls::MEMORY); 
			fwrite(&x, sizeof (x), 1, bur); 
		}
} checker_result;
////////////////////////////////////////////////////////////////////////////////////////////


int main (int argc, char* argv[]){
	// Открываем файл входных данных, файл выходных данных и ответ участника
/*	FILE * bui = fopen("matr.in", "rb");
	FILE * buo = fopen("matr.out", "rb");
	FILE * bua = fopen("answer.txt", "rb");
	int n;
	// Считываем размерность матриц
	fread(&n, sizeof (n), 1, bui);
	// Выделяем память для матрицы ответа жюри и ответа участника
	double * ans = new double[n * n], *res = new double[n * n];
	double ans_time, res_time;
	// Считываем время работы программы участника и матрицу участника
	fread(&res_time, sizeof (res_time), 1, buo);
	fread(res, sizeof (*res), n * n, buo);
	// Считываем время работы программы и матрицу жюри
	fread(&ans_time, sizeof (ans_time), 1, bua);
	fread(ans, sizeof (*ans), n * n, bua);
	// Вычисляем ошибку, как квадрат нормы разности решений
	double diff = 0.0;
	for (int i = 0; i < n * n; i++)
		diff += (ans[i] - res[i]) * (ans[i] - res[i]);*/
	std::ifstream in1(argv[1], std::ios::binary); //perfect
	std::ifstream in2(argv[2], std::ios::binary); //result 
	
	bool success1,success2;
	int AC = -1;
    double time1, time2;
    int MSize1, MSize2;
    int VNum1, VNum2;
    in1.read((char*)&success1, sizeof(success1));
    in2.read((char*)&success2, sizeof(success2));
    if(success1 != success2){ 
    	checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
    }else{
    	if((success1 == success2) && (success1 == 0)){
    	    // graph is not connected
    		checker_result.write_message("AC. Both graphs is not connected.");
    		checker_result.write_verdict(verdict::AC);
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
				checker_result.write_verdict(verdict::WA);
    		}
		}
	}

	// Проверяем, что ошибка мала,тогда сообщаем, что решение корректно, иначе - некорректно.
	if (AC == 0){
		checker_result.write_message("AC. Numbers are equal.");
		checker_result.write_verdict(verdict::AC);
	}else{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
	}

	checker_result.write_time (time2 * 1e7);
	in1.close();
	in2.close();
	return 0;
}