#include "Person.h"

Person::Person() :
	id(time(nullptr)),
	firstName(""),
	midName(""),
	lastName(""),
	age(0)
{}
Person::Person(string firstname, string lastname, int age_, ID_t id_) :
	id(id_),
	firstName(firstname),
	midName(""),
	lastName(lastname),
	age(age_)
{}
Person::Person(string firstname, string midname, string lastname, int age_, ID_t id_) :
	Person(firstname, lastname, age_, id_)
{
	midName = midname;
}
ID_t Person::GetID()
{
	return id;
}
string Person::GetFirstName()
{
	return firstName;
}
string Person::GetMiddleName()
{
	return midName;
}
string Person::GetLastName()
{
	return lastName;
}
void Person::SetFirstName(string firstname)
{
	firstName = firstname;
}
void Person::SetMiddleName(string midname)
{
	midName = midname;
}
void Person::SetLastName(string lastname)
{
	lastName = lastname;
}
void Person::SetID(ID_t id_)
{
	id = id_;
}
int Person::GetAge()
{
	return age;
}
void Person::SetAge(int age_)
{
	if (age_ < 0)
		throw std::invalid_argument("Age cannot be negative!");
	else
		age = age_;
}