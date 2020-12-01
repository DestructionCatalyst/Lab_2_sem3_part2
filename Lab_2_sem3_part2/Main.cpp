#include <fstream>
#include <cstdlib>

#include "CachedSequence.h"
#include "Person.h"
#include "DoubleLinkedList.h"

#include "dependencies/TestEnvironment.h"
#include "dependencies/TimedTest.h"

#define FIRST_NAMES_COUNT 5162
#define LAST_NAMES_COUNT 8879

#define TEST_SAMPLE_SIZE 30000
#define TEST_CACHE_SIZE 50

#define TEST_FREQUENT_COUNT 5
#define TEST_FREQUENT_PERCENT 80

#define TEST_QUERIES_COUNT 5000

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

void showName(Person* p)
{
	std::cout << p->GetFirstName() << " " << p->GetLastName() << std::endl;
}

void testDoubleList()
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

void testCache(Timer& t)
{
	CachedSequence<int, Person*>* seq = new CachedSequence<int, Person*>(
		[](int id, int tableSize)->int
		{
			return id % tableSize;
		},
		TEST_CACHE_SIZE);

	Person* p;

	std::cout << "Generating data..." << std::endl;

	std::ifstream inFirst("firstnames.txt", /*std::ios::binary|*/std::ios::in);
	std::ifstream inLast("us.txt", /*std::ios::binary|*/std::ios::in);

	string firstname;
	string lastname;

	for (int i = 0; i < TEST_SAMPLE_SIZE; i++)
	{
		inFirst >> firstname;
		inLast >> lastname;

		seq->Add(i, new Person(firstname, lastname, rand() % 100, i));
	}

	inFirst.close();
	inLast.close();

	ArraySequence<int>* FAQ = new ArraySequence<int>();

	for (int i = 0; i < TEST_FREQUENT_COUNT; i++)
	{
		FAQ->Append(rand() % TEST_SAMPLE_SIZE);
	}

	std::cout << "Data generated!" << std::endl;

	Person* gotten;

	t.Start();

	for (int i = 0; i < TEST_QUERIES_COUNT; i++)
	{
		if (rand() % 100 < TEST_FREQUENT_PERCENT)
		{
			//From FAQ
			gotten = seq->Get(FAQ->Get(rand() % TEST_FREQUENT_COUNT));
		}
		else
		{
			//Random
			gotten = seq->Get(rand() % TEST_SAMPLE_SIZE);
		}
		std::cout << gotten->GetLastName() << ' ';
	}

	t.Pause();
	
	std::cout << std::endl;
}

Person* findInSequence(int id, ArraySequence<Person*>* seq)
{
	Person* tmp;

	for(int i = 0; i < seq->GetLength(); i++)
	{ 
		tmp = seq->Get(i);
		if (tmp->GetID() == id)
			return tmp;
	}

	return nullptr;
}

void testLinearSearch(Timer& t)
{
	ArraySequence<Person*>* seq = new ArraySequence<Person*>();

	Person* p;

	std::cout << "Generating data..." << std::endl;

	std::ifstream inFirst("firstnames.txt", /*std::ios::binary|*/std::ios::in);
	std::ifstream inLast("us.txt", /*std::ios::binary|*/std::ios::in);

	string firstname;
	string lastname;

	for (int i = 0; i < TEST_SAMPLE_SIZE; i++)
	{
		inFirst >> firstname;
		inLast >> lastname;

		seq->Append(new Person(firstname, lastname, rand() % 100, i));
	}

	inFirst.close();
	inLast.close();

	ArraySequence<int>* FAQ = new ArraySequence<int>();

	for (int i = 0; i < TEST_FREQUENT_COUNT; i++)
	{
		FAQ->Append(rand() % TEST_SAMPLE_SIZE);
	}

	std::cout << "Data generated!" << std::endl;

	Person* gotten;

	t.Start();

	for (int i = 0; i < TEST_QUERIES_COUNT; i++)
	{
		if (rand() % 100 < TEST_FREQUENT_PERCENT)
		{
			//From FAQ
			gotten = findInSequence(FAQ->Get(rand() % TEST_FREQUENT_COUNT), seq);
		}
		else
		{
			//Random
			gotten = findInSequence(rand() % TEST_SAMPLE_SIZE, seq);
		}

		std::cout << gotten->GetLastName() << ' ';
	}

	std::cout << std::endl;
	t.Pause();
}

int main()
{
	srand(clock());

	UnitTest("Double linked list test", testDoubleList).Run();
	TimedTest("Cache test", testCache).Run();
	TimedTest("Linear search for comparsion", testLinearSearch).Run();

	/*

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

		showName(p);
	}

	std::cout << std::endl;

	showName(seq->Get(5));
	showName(seq->Get(1));
	showName(seq->Get(7));
	showName(seq->Get(0));
	showName(seq->Get(5));
	//ShowName(seq->Get(0));
	showName(seq->Get(3));
	showName(seq->Get(2));
	showName(seq->Get(5));
	showName(seq->Get(0));
	showName(seq->Get(5));
	showName(seq->Get(5));
	showName(seq->Get(4));
	showName(seq->Get(5));
	showName(seq->Get(8));
	showName(seq->Get(6));
	showName(seq->Get(5));

	*/

	return 0;
}