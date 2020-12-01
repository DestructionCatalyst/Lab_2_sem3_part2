#include <fstream>
#include <cstdlib>

#include "CachedSequence.h"
#include "Person.h"
#include "DoubleLinkedList.h"

#include "dependencies/TestEnvironment.h"

#define FIRST_NAMES_COUNT 5162
#define LAST_NAMES_COUNT 8879

void thinOutTheLastNames()
{
	std::ifstream in("us.txt", /*std::ios::binary|*/std::ios::in);
	std::ofstream out("lastnames.txt", /*std::ios::binary|*/std::ios::out);

	int i = 0;
	string tmp = "";

	while (in)
	{
		in >> tmp;

		if (i % 10 == 0)
			out << tmp << '\n';

		i++;
	}

	in.close();
	out.close();
}

string getLineFromFile(int lineNumber, string filename)
{
	string res;

	std::ifstream inFirst(filename, /*std::ios::binary|*/std::ios::in);

	for (int i = 0; i < lineNumber; i++)
		inFirst >> res;

	inFirst.close();

	return res;
}

Person* generatePerson(long long number = -1)
{
	int id = 0;

	if (number < 0)
		id = rand();
	else 
		id = number;

	int nameNumber = rand() % FIRST_NAMES_COUNT + 1;

	string firstname = getLineFromFile(nameNumber, "firstnames.txt");

	nameNumber = rand() % LAST_NAMES_COUNT + 1;

	string lastname = getLineFromFile(nameNumber, "lastnames.txt");

	int age = rand() % 100;

	return new Person(firstname, lastname, age, id);
}

void ShowName(Person* p)
{
	std::cout << p->GetFirstName() << " " << p->GetLastName() << std::endl;
}

void TestDoubleList()
{
	DoubleLinkedList<int>* list = new DoubleLinkedList<int>();

	list->Append(5);
	list->Append(100);
	list->Append(3);
	list->Prepend(2);
	list->Prepend(0);
	list->Append(20);
	list->Append(4);
	list->Prepend(1000);

	DoubleNode<int>* ptr = list->Tail();

	while (ptr->GetContent() != 100)
	{
		ptr = ptr->Prev();
	}

	list->Remove(ptr);


	ptr = list->Head();

	while (ptr->GetContent() != 3)
	{
		ptr = ptr->Next();
	}

	list->MoveToStart(ptr);

	ptr = list->Head();

	int exp[] = { 3, 1000, 0, 2, 5, 20, 4 };
	int index = 0;

	while (ptr->Next() != nullptr)
	{
		ASSERT_EQUALS(ptr->GetContent(), exp[index]);

		index++;
		ptr = ptr->Next();
	}
}

int main()
{
	srand(clock());

	UnitTest("Double linked list test", TestDoubleList).Run();

	CachedSequence<int, Person*>* seq = new CachedSequence<int, Person*>(
		[](int id, int tableSize)->int
		{
			return id % tableSize;
		},
		5);

	Person* p;

	for (int i = 0; i < 10; i++) {
		 p = generatePerson(i);

		seq->Add(p->GetID(), p);

		ShowName(p);
	}

	std::cout << std::endl;

	ShowName(seq->Get(5));
	ShowName(seq->Get(1));
	ShowName(seq->Get(7));
	ShowName(seq->Get(0));
	ShowName(seq->Get(5));
	//ShowName(seq->Get(0));
	ShowName(seq->Get(3));
	ShowName(seq->Get(2));
	ShowName(seq->Get(5));
	ShowName(seq->Get(0));
	ShowName(seq->Get(5));
	ShowName(seq->Get(5));
	ShowName(seq->Get(4));
	ShowName(seq->Get(5));
	ShowName(seq->Get(8));
	ShowName(seq->Get(6));
	ShowName(seq->Get(5));



	return 0;
}