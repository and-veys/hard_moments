#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <ctime>
#include <Math.h>
#include <iomanip>
using namespace std;
//-----------------------------------------------------
void pushArithmeticMean(list<double>& l) {
	double res = 0;
	for(const auto el : l)
		res+=el;
	l.push_back(res/l.size());
}
template <typename T>
void print_conteiner(T& l) {
	for(const auto& el : l)
		cout << el << " ";
	cout << endl;
}
//-----------------------------------------------------
class Matrix {
private:
	vector<int> data;
	int size;
	random(int min, int max) {return (rand()%(max-min) + min);}
	Matrix fillMatrixNew(int ind);
public:
	Matrix(){};
	void fillMatrixRandom(int sz, int min, int max);
	void printMatrix();
	int getDeterminant();
	void push_back(int n){data.push_back(n);}
};
void Matrix::fillMatrixRandom(int sz, int min, int max) {
	srand(time(NULL)); 
	size = sz;
	for(int i=0; i<sz*sz; i++)
		data.push_back(random(min, max));
}
void Matrix::printMatrix() {
	int i = 0;
	for(const auto& el : data) {
		cout << setw(4) << el;
		i++;
		if(i==size){
			cout << endl;
			i=0;
		}
	}
	cout << endl;
}
int Matrix::getDeterminant(){
	if(size == 1) return data.front();
	int dt = 0;
	for(int i=0; i<size; ++i){
		Matrix temp = fillMatrixNew(i);
		dt += data[i] * temp.getDeterminant() * (i%2==0?1:-1);
	}
	return dt;
}
Matrix Matrix::fillMatrixNew(int ind) {
	int i = 0;
	Matrix res;
	for(auto p = data.begin()+size; p<data.end(); ++p){
		if(i != ind) 
			res.push_back(*p);
		i++;
		if(i==size)
			i=0;
	}
	res.size = size-1;
	return res;		
}
//-----------------------------------------------------
class My_class {
private:
	vector<int> data;
public:
	My_class(vector<int>& v):data(v){}
	vector<int>::iterator begin(){return data.begin();}
	vector<int>::iterator end(){return data.end();}
}; 
//-----------------------------------------------------
int main() {
	cout << endl << "Exercise #1" << endl;
	list<double> lst = {1.5, 0.2, -3.0, 4.6, 5.1};
	pushArithmeticMean(lst);
	print_conteiner(lst);
	
	cout << endl << "Exercise #2" << endl;
	Matrix mx;
	mx.fillMatrixRandom(5, 0, 100);
	mx.printMatrix();
	cout << "Matrix\'s determinant is " << mx.getDeterminant() << endl;
	
	cout << endl << "Exercise #3" << endl;
	vector<int> vec = {8, 4, 0, 2, 6};
	My_class mcl(vec);
	sort(mcl.begin(), mcl.end());
	print_conteiner(mcl);
	
	return 0;
}