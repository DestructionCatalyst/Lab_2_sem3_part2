#include <fstream>

#include "CachedSequence.h"
#include "Person.h"

int main()
{
	CachedSequence<int, Person*>* seq = new CachedSequence<int, Person*>(
		[](int id, int tableSize)->int
		{
			return id % tableSize;
		},
		10);

	std::ifstream in("us.txt", /*std::ios::binary|*/std::ios::in);
	std::ofstream out("surnames.txt", /*std::ios::binary|*/std::ios::out);

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

	return 0;
}