#include <iostream>
#include <optional>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#define LENGHT_FIELD_NAME		20

using namespace std;
typedef struct Person {
	string first_name;	
	optional<string> patronymic;
	string last_name;
	Person(string l, string f, optional<string> p = nullopt): last_name(l), first_name(f), patronymic(p) {}
	friend ostream& operator << (ostream& out, Person& p);
	bool operator < (const Person& p) {return (tie(last_name, first_name, patronymic) < tie(p.last_name, p.first_name, p.patronymic));}
	bool operator == (const Person& p) {return (tie(last_name, first_name, patronymic) == tie(p.last_name, p.first_name, p.patronymic));}
	bool operator == (const string& ln) {return last_name == ln;}
} Person;

typedef struct PhoneNumber {
	int country;
	int city;
	string number;
	optional<int> additional; 
	PhoneNumber(int co, int ci, string n, optional<int> a = nullopt): country(co), city(ci), number(n), additional(a){};  
	friend ostream& operator << (ostream& out, PhoneNumber& pn);	
	bool operator < (const PhoneNumber& pn) {return (tie(country, city, number, additional) < tie(pn.country, pn.city, pn.number, pn.additional));}
	bool operator == (const PhoneNumber& pn) {return (tie(country, city, number, additional) == tie(pn.country, pn.city, pn.number, pn.additional));}
	void operator = (const PhoneNumber& pn){country = pn.country; city = pn.city; number = pn.number; additional = pn.additional;}
} PhoneNumber;

ostream& operator << (ostream& out, Person& p) {	
	return out << setw(LENGHT_FIELD_NAME) << p.last_name << setw(LENGHT_FIELD_NAME) << p.first_name << setw(LENGHT_FIELD_NAME) << p.patronymic.value_or("");	
}

ostream& operator << (ostream& out, PhoneNumber& pn) {
	if(pn.additional.has_value())
		return out << "+" << pn.country << "(" << pn.city << ")" << pn.number << " " << pn.additional.value();		
	else
		return out << "+" << pn.country << "(" << pn.city << ")" << pn.number;
}

class PhoneBook {
private:
	vector<pair <Person*, PhoneNumber*>> notes;
public:
	PhoneBook(ifstream& f);
	void SortByPhone();
	void SortByName();
	tuple<string, PhoneNumber*> GetPhoneNumber(const string& ln);
	void ChangePhoneNumber(Person p, PhoneNumber pn);
	~PhoneBook();
	friend ostream& operator << (ostream& out, PhoneBook& pb);	
};
ostream& operator << (ostream& out, PhoneBook& pb) {	
	for(pair n : pb.notes)
	{
		out << *n.first << "   " << *n.second << endl;
	}
	return out;
}
void PhoneBook::SortByName() {
	sort(notes.begin(), notes.end(), [](pair <Person*, PhoneNumber*>& lhs, pair <Person*, PhoneNumber*>& rhs){return *lhs.first < *rhs.first;});
}
void PhoneBook::SortByPhone() {
	sort(notes.begin(), notes.end(), [](pair <Person*, PhoneNumber*>& lhs, pair <Person*, PhoneNumber*>& rhs){return *lhs.second < *rhs.second;});	
}
tuple<string, PhoneNumber*> PhoneBook::GetPhoneNumber(const string& ln)
{
	int counter=0;
	PhoneNumber* temp;
	for(pair n : notes)
	{
		if(*n.first == ln)
		{
			temp = n.second;
			counter++;	
		}
	}	
	if(counter==0) return tie("not found", temp);
	else if(counter==1) return tie("", temp);
	else return tie("found more than 1", temp);
}
void PhoneBook::ChangePhoneNumber(Person p, PhoneNumber pn)
{
	for(pair n : notes)
	{
		if(*n.first == p)
		{
			*n.second = pn;			
			break;
		}	
	}
}
PhoneBook::~PhoneBook() {
	for(pair n : notes)
	{
		delete n.first;
		delete n.second;
	}
}
PhoneBook::PhoneBook(ifstream& f) {
	string str;
	vector<string> vec;
	stringstream ss;
	Person* p;
	PhoneNumber* pn;
	while(!f.eof())
	{
        getline(f, str);
		ss << str;
		while (getline(ss, str, ' ')) 
			vec.push_back(str);
		if(static_cast<int>(vec.size()) == 7)
		{		
			if(vec[2] == "-") 
				p = new Person(vec[0], vec[1]);
			else 
				p = new Person(vec[0], vec[1], vec[2]);
			if(vec[6] == "-")
				pn = new PhoneNumber(stoi(vec[3]), stoi(vec[4]), vec[5]);
			else
				pn = new PhoneNumber(stoi(vec[3]), stoi(vec[4]), vec[5], stoi(vec[6]));
			notes.push_back({p, pn});
			
		}
		vec.clear();
		ss.clear();
	}	
}
int main() {
	
	ifstream file("PhoneBook.txt"); 
   	PhoneBook book(file);
	cout << book;
	
	cout << "------SortByPhone-------" << endl;
   	book.SortByPhone();
   	cout << book;
 
   	cout << "------SortByName--------" << endl;
   	book.SortByName();
   	cout << book;	
	
	cout << "-----GetPhoneNumber-----" << endl;
   	auto print_phone_number = [&book](const string& surname) { 
   	   	cout << setw(LENGHT_FIELD_NAME) << surname << "   "; 
   	   	auto answer = book.GetPhoneNumber(surname); 
   	   	if (get<0>(answer).empty()) 
   	   	   	cout << *(get<1>(answer)); 
   	   	else 
   	   	   	cout << get<0>(answer); 
   	   	   	cout << endl; 
   	};
   	print_phone_number("Ivanov");
   	print_phone_number("Petrov");
	print_phone_number("Solovev");

   	cout << "----ChangePhoneNumber----" << endl;
   	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{7, 123, "15344458", nullopt});
   	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
   	cout << book;
	
	return 0;
}