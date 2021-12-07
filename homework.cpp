#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <ctime>
#include <list>
#include <iomanip>
using namespace std;
mutex m;
class pcout : public ostringstream {
public:		
		~pcout() {
			lock_guard lg(m);
			cout << this->str();
		}
};
void doSomething(string str, int num) {	
	pcout() << "Start: " << str << endl;		
	this_thread::sleep_for(1s);					
	pcout() << "End: " << num << endl;					
}
//---------------------
void timer(int & it, int k) {
	string s = string("\r") + string(20, ' ') + "\r";
	while(true) {
		cout << s << "Progress: " << ((int)(100.0*it/k)) << "%";
		if(it == k) break;
		this_thread::sleep_for(600ms);	
	}
}
void primeNumber(int k) {
	int num = 1;
	int it = 0;
	int i;
	thread th4_T(timer, ref(it), k);
	while(it != k) {	
		++num;
		for(i=2; i<num; i++) {
			if(num%i == 0) break;	
		}
		if(i==num) ++it;
	}
	th4_T.join();
	cout << endl << "Result: " << num << endl;
}
//--------------------- 
int random() {
	return rand()%90 + 10;
}
void printList(list<int> & lst) {
	cout << string("\r") + string(60, ' ') + "\r";
	for_each(lst.begin(), lst.end(), [&](auto v){cout << setw(2) << v << "$ ";});	
}
void owner(list<int> & lst) {
	while(true) {
		this_thread::sleep_for(1s);	
		lock_guard lg(m);
		if(lst.size() == 0) break;
		lst.push_back(random());
		printList(lst);
	}
}
void thief(list<int> & lst) {
	while(true) {
		this_thread::sleep_for(600ms);	
		lock_guard lg(m);
		if(lst.size() == 0) break;
		lst.erase(max_element(lst.begin(), lst.end()));		
		printList(lst);		
	}
}
int main() {
	cout << endl << "_____Exercise #1_____" << endl;
	thread th1(doSomething, "th_1", 1);
   	thread th2(doSomething, "th_2", 2);
   	thread th3(doSomething, "th_3", 3);
   	th1.join();
   	th2.join();
   	th3.join();
	
	cout << endl << "_____Exercise #2_____" << endl;
	thread th4(primeNumber, 10'000); 
	th4.join();
	
	cout << endl << "_____Exercise #3_____" << endl;
	list<int> lst(10);
	srand(time(NULL));
	generate (lst.begin(), lst.end(), random);
	thread th5(owner, ref(lst)); 
	thread th6(thief, ref(lst)); 
	th5.join();
   	th6.join();	
	return 0;
}