#pragma once
#include <string>
#include <ctime>
#include <stdexcept>

using std::string;

typedef unsigned long long ID_t;

class Person {
private:
	ID_t id;
	string firstName;
	string midName;
	string lastName;
	int age;
public:
	Person();
	Person(string firstname, string lastname, int age_, ID_t id_ = time(nullptr));
	Person(string firstname, string midname, string lastname, int age_, ID_t id_ = time(nullptr));
public:
	ID_t GetID();
	string GetFirstName();
	string GetMiddleName();
	string GetLastName();
	void SetFirstName(string firstname);
	void SetMiddleName(string midname);
	void SetLastName(string lastname);
	void SetID(ID_t id_);
	int GetAge();
	void SetAge(int age);

};



