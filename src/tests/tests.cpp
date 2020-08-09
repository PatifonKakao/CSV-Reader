#include "CSVReader.h"


int main(int argc, char** argv)
{
	auto csv = CSVReader(argv[1]);
	csv.print();

	return 0;
}