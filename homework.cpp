//22 ноября 2021
#include <any>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Timer.h"
#define MAX_POINTER		10

using namespace std;
//--------------------------------------------------
template <typename T>
void printSwap(T* x, T* y) {
	cout << setw(MAX_POINTER) << x << setw(5) << *x << setw(MAX_POINTER) << y << setw(5) << *y <<endl;
}

template <typename T>
void mySwap(T** x, T** y) {
		T* temp = *x;
		*x = *y;
		*y = temp;;
}
//--------------------------------------------------
template <typename T>
void printVector(vector<T>& v) {
	for(T el : v)
		cout << setw(MAX_POINTER) << el << setw(4) << *el;
	cout << endl;
}

template <typename T>
void mySort(vector<T>& v) {
	sort(v.begin(), v.end(), [](T lhs, T rhs){return *lhs < *rhs;});
}	
//--------------------------------------------------
int Count_Find(ifstream& f) {
	int counter = 0;
	string str;
	string vm = "aeiouyAEIOUY";
	while(!f.eof()) {
        getline(f, str);
		counter += count_if(str.begin(), str.end(), [&vm](char n){return vm.find(n) != -1;});		 
	}
	return counter;
}

int Count_For(ifstream& f) {
	int counter = 0;
	string str;
	string vm = "aeiouyAEIOUY";
	while(!f.eof()) {
        getline(f, str);
		counter += count_if(str.begin(), str.end(), [&vm](char n){
			for(char ch : vm) 
					if(n == ch) return true;		
			return false;
		});		 
	}
	return counter;
}

int For_Find(ifstream& f) {
	int counter = 0;
	string str;
	string vm = "aeiouyAEIOUY";
	int n;
	while(!f.eof()) {
        getline(f, str);
		for(char ch : vm) {
			n=0;
			while(true){				
				n = str.find(ch, n)+1;
				if(n == 0) break;
				++counter;
			}	
		}
	}
	return counter;
}

int For_For(ifstream& f) {
	int counter = 0;
	string str;
	string vm = "aeiouyAEIOUY";
	while(!f.eof()) {
        getline(f, str);
		for(char ch : vm) {
			for(char s : str)
				if(ch == s) ++counter;
		}
	}
	return counter;	
}

int main() {
	
	cout << "Exercise_1" << endl;
	int * a = new int(6);
	int * b = new int(7);
	printSwap(a, b);
	mySwap(&a, &b);
	printSwap(a, b);
	delete a;
	delete b;
	
	cout << "Exercise_2" << endl;
	srand(time(NULL));
	vector<int *> vec;
	for(size_t i=0; i<10; ++i) 
		vec.push_back(new int(rand()%100));
	printVector(vec);
	mySort(vec);
	printVector(vec);
	for(size_t i=0; i<vec.size(); ++i) 
		delete vec[i];	
	
	cout << "Exercise_3" << endl;
	ifstream file("War_and_Peace.txt"); 
	Timer tm("Count - Find");
	cout << Count_Find(file) << "   ";	
	tm.print();
	file.close();							//Что-то seekg не получилось ((
	file.open("War_and_Peace.txt");
	tm.start("Count - For");
	cout << Count_For(file) << "   ";	
	tm.print();
	file.close();
	file.open("War_and_Peace.txt");
	tm.start("For - Find");
	cout << For_Find(file) << "   ";
	tm.print();
	file.close();
	file.open("War_and_Peace.txt");
	tm.start("For - For");
	cout << For_For(file) << "   ";
	tm.print();
	
	return 0;
}