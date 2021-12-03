#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
template <typename TT, typename T>
void uniqueElements(T begin, T end) {
	set<TT> temp;
	T it = begin;
	while(it != end) {
		temp.insert(*it);
		it = next(it);		
	}
	for(auto & i : temp)
		cout << i << endl;
}
void addSentences(string & str, multimap<int, string> & mmap) {
	auto it = find_if(str.begin(), str.end(),[](char ch){return ch!=' ';});		//trim left
	if(it != str.end()) str = str.substr(it - str.begin());
	
	it = find_if(str.begin(), str.end(), [](char ch){return ch=='.' || ch=='?' || ch=='!';});
	if(it == str.end()) return;
	string temp = str.substr(0, it - str.begin() + 1);
	str = str.substr(temp.size());	
	mmap.insert({temp.size(), move(temp)});				
	addSentences(str, mmap);
}
int main() {
	cout << endl << "____Exercise #1____" << endl << endl;
	vector<string> v_string = {"one", "three", "five", "one", "two", "two", "four", "six", "five", "seven"};
	uniqueElements<string>(v_string.begin(), v_string.end()); 
	list<double> l_double = {1.1, 3.3, 5.5, 1.1, 2.2, 2.2, 4.4, 6.6, 5.5, 7.7};
	uniqueElements<double>(l_double.begin(), l_double.end()); 
	
	cout << endl << "____Exercise #2____" << endl << endl;
	string buffer;
	string str = "";
	multimap<int, string> mmap;
	cout << "Enter some text. Exit is \"###\":" << endl;
	while(true){
		getline(cin, buffer);
		if(buffer == "###") break;
		str += buffer;	
		addSentences(str, mmap);
	}	
	for(auto i : mmap) cout << i.second << endl;	
	cout << endl << "And string without ending: \"" << str << "\"" << endl;
	
	return 0;
}