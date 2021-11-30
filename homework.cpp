#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <ctime>
#include <numeric>
using namespace std;

template <typename T> 
void print(T & cont) {
	for_each(cont.begin(), cont.end(), [](auto i){cout << i << ' ';});
	cout << endl;
}
template <typename T, typename TN> 						//для vector, deque, list				
void insert_sorted(T & cont, TN n) {
	int it; 
	if(cont.front()  < cont.back()) 
		it = distance(cont.begin(), find_if(cont.begin(), cont.end(), [n](int i){return i > n;}));
	else 
		it = distance(cont.begin(), find_if(cont.begin(), cont.end(), [n](int i){return i < n;}));
	cont.insert(next(cont.begin(), it), n);
}
template <typename T, typename TN> 
void ex_1(T & cont, TN a1, TN a2) {
	print(cont);
	insert_sorted(cont, a1);
	print(cont);
	reverse(cont.begin(), cont.end());
	print(cont);
	insert_sorted(cont, a2);
	print(cont);
	cout << endl;	
}
int main() {
	cout << endl << "____Exercise #1____" << endl << endl;
	vector<int> vec = {10, 20, 20, 30, 30, 40, 50, 50, 60, 60};
	deque<double> deq = {10.1, 20.2, 20.2, 30.3, 30.3, 40.4, 50.5, 50.5, 60.6, 60.6};
	list<char> lst = {'a', 'd', 'd', 'g', 'g', 'j', 'm', 'm', 'p', 'p'};
	ex_1(vec, 25, 55);
	ex_1(deq, 25.5, 55.5);
	ex_1(lst, 'h', 'e');	
	
	cout << endl << "____Exercise #2____" << endl << endl;
	vector<double> vec_A(100);
	vector<int> vec_B(100);
	srand(time(NULL));
	generate (vec_A.begin(), vec_A.end(), [](){return (1000.0*rand()/RAND_MAX);});
	transform(vec_A.begin(), vec_A.end(), vec_B.begin(), [](double i){return (int)i;});
	print(vec_A);
	cout << endl;
	print(vec_B);
	transform(vec_A.begin(), vec_A.end(), vec_A.begin(), [](double i){return (i-(int)i)*(i-(int)i);});
	cout << endl << "Deviation = " << accumulate(vec_A.begin(), vec_A.end(), 0.0) << endl;	
	return 0;
}