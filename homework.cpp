#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <numeric>
#include <tuple>
#include <algorithm>

using namespace std;
class FullName {
private:
	string lastname;
	string firstname;
	string patronymic;
	void saveString(string & str, ofstream & out);
	void openString(string & str, ifstream & in);
public:
	FullName(){}
	FullName(string ln, string fn, string pt = "") : lastname(ln), firstname(fn), patronymic(pt){}
	string getFullName() {return lastname + " " + firstname + (patronymic == ""?"":" ") + patronymic;}	
	bool operator == (const FullName & st){
		return tie(lastname, firstname, patronymic) == tie(st.lastname, st.firstname, st.patronymic);
	};
	void open(ifstream & in);
	void save(ofstream & out);
};
void FullName::saveString(string & str, ofstream & out){
	int len = str.size();
	out.write(reinterpret_cast<char*>(&len), sizeof(len));
	out.write(str.c_str(), len);
}
void FullName::openString(string & str, ifstream & in) {
	int len;
	in.read(reinterpret_cast<char*>(&len), sizeof(len));
	str.resize(len);
	in.read(str.data(), len);
}
void FullName::save(ofstream & out) {
	saveString(lastname, out);
	saveString(firstname, out);
	saveString(patronymic, out);
}
void FullName::open(ifstream & in) {
	openString(lastname, in);
	openString(firstname, in);
	openString(patronymic, in);
}
//---------------------------------------------------------------
class Student {
private:
	FullName fullName;
	vector<char> score;
	double averageScore;
public:
	Student(){}
	Student(FullName fn) : fullName(move(fn)) {}
	string getAllInfo() {		
		return fullName.getFullName() + ": " + getScore() + " (average score: " + to_string(averageScore) + ")";
	}
	string getScore() {
		string res = "";
		for(auto i : score) res += to_string(i);
		return res;
	}
	void addScore(int sc) {			
		score.push_back(static_cast<char>(sc));
		averageScore = static_cast<double>(accumulate(score.begin(), score.end(), 0)) / score.size();
	}
	bool operator == (const FullName & st){return fullName == st;};
	double getAverageScore(){return averageScore;}
	void open(ifstream & in);
	void save(ofstream & out);
};
void Student::save(ofstream & out) {
	fullName.save(out);
	int len = score.size();
	out.write(reinterpret_cast<char*>(&len), sizeof(len));
	for(char ch : score)
		out.write(&ch, sizeof(ch));
}
void Student::open(ifstream & in) {
	fullName.open(in);
	int len;
	char ch;
	in.read(reinterpret_cast<char*>(&len), sizeof(len));
	for(int i=0; i<len; ++i) {
		in.read(&ch, sizeof(ch));
		addScore(ch);
	}
}
//---------------------------------------------------------------
class IRepository {
	virtual void open(ifstream & in) = 0; // бинарная десериализация в файл
	virtual void save(ofstream & out) = 0; // бинарная сериализация в файл
};
class IMethods {
	virtual double getAverageScore(const FullName& name) = 0;
	virtual string getAllInfo(const FullName& name) = 0;
	virtual string getAllInfo() = 0;
};
class StudentsGroup : public IMethods, public IRepository {
private:
	vector<Student> group;
	auto findStudent(const FullName& fn) { 
		return find_if(group.begin(), group.end(), [&](auto st){return st == fn;});
	}
public:
	StudentsGroup(){}
	void addStudent(Student st){group.push_back(st);}
	auto begin(){return group.begin();}
	auto end(){return group.end();}
	string getAllInfo() {
		string res;
		for(auto & st : group)
			res += st.getAllInfo() + "\n";
		return res;
	}
 	string getAllInfo(const FullName& fn) {return findStudent(fn)->getAllInfo();}
	double getAverageScore(const FullName& fn) {return findStudent(fn)->getAverageScore();}
	void open(ifstream & in);
	void save(ofstream & out);
};
void StudentsGroup::open(ifstream & in) {	
	int len;
	in.read(reinterpret_cast<char*>(&len), sizeof(len));
	for(int i=0; i<len; ++i) {
		Student st;
		st.open(in);
		addStudent(st);
	}
}
void StudentsGroup::save(ofstream & out) {
	int len = group.size();
	out.write(reinterpret_cast<char*>(&len), sizeof(len));
	for(auto st : group)
		st.save(out);
}
//---------------------------------------------------------------
int random(int min, int max) {
	return rand()%(max-min+1) + min;
}
void generationScore(Student & st) {
	vector<char> sc;
	int n = random(3, 10);
	for(int i=0; i<n; ++i) st.addScore(random(2, 5));	
}
int main() {
	srand(time(NULL));
	
	StudentsGroup students;	
	students.addStudent(Student(FullName("Ivanov", "Aleksey", "Sergeevich")));
	students.addStudent(Student(FullName("Petrov", "Ivan")));
	students.addStudent(Student(FullName("Sidorova", "Anna", "Andreevna")));
	
	for(auto & st : students)
		generationScore(st);

	cout << "---------------------------------------------" << endl;	
	cout << students.getAllInfo(FullName("Petrov", "Ivan")) << endl;
	cout << students.getAverageScore(FullName("Petrov", "Ivan")) << endl;
	cout << "---------------------------------------------" << endl;
	cout << students.getAllInfo() << endl;	
	cout << "---------------------------------------------" << endl;
	
	ofstream out("students.bin", ios::binary);
	if(out.is_open()) {
		students.save(out);
		out.close();
	} else
		cout << "Save Error !!" << endl;
	
	StudentsGroup students2;
	ifstream in("students.bin", ios::binary);	
	if(in.is_open()) {
		students2.open(in);
		in.close();
	} else
		cout << "Open Error !!" << endl;	
	cout << students2.getAllInfo() << endl;
	cout << "---------------------------------------------" << endl;
	return 0;
}