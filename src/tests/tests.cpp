#include "CSVReader.h"
#include <gtest/gtest.h>

TEST(test, test1)
{
	std::ofstream file;
	file.open("test1.csv");
	std::string str(
		",A,B,Cell\n1,1,0,1\n2,2,=A1+Cell30,0\n30,0,=B1+A1,5\n"
	);
	file << str;
	file.close();

	std::string ref(
		",A,B,Cell\n1,1,0,1\n2,2,6,0\n30,0,1,5\n"
	);

	CSVReader csv("test1.csv");
	
	EXPECT_TRUE(csv.getCalcData() == ref);

	std::cout << csv.getCalcData();
}

TEST(test, test2)
{
	std::ofstream file;
	file.open("test2.csv");
	std::string str(
		",A,B,Cell\n1,1,0,1\n2,2,=A1+B30,0\n30,0,=B1+A1,5\n"
	);
	file << str;
	file.close();

	std::string ref(
		",A,B,Cell\n1,1,0,1\n2,2,2,0\n30,0,1,5\n"
	);

	CSVReader csv("test2.csv");
	std::cout << csv.getCalcData();

	EXPECT_TRUE(csv.getCalcData() == ref);
}

TEST(test, test3)
{
	std::ofstream file;
	file.open("test3.csv");
	std::string str(
		",A,B,Cell\n1,1,0,1\n2,,=A1+B30,0\n30,0,=B1+A1,5\n"
	);
	file << str;
	file.close();

	std::string ref(
		",A,B,Cell\n1,1,0,1\n2,,2,0\n30,0,1,5\n"
	);

	CSVReader csv("test3.csv");
	std::cout << csv.getCalcData();

	EXPECT_TRUE(csv.getCalcData() == ref);
}

TEST(test, test4)
{
	std::ofstream file;
	file.open("test4.csv");
	std::string str(
		",A,B,Cell\n1,1,0,1\n2,,=A1+B30,0\n30,0,=B1+A1,=B2*10\n"
	);
	file << str;
	file.close();

	std::string ref(
		",A,B,Cell\n1,1,0,1\n2,,2,0\n30,0,1,20\n"
	);

	CSVReader csv("test4.csv");
	std::cout << csv.getCalcData();

	EXPECT_TRUE(csv.getCalcData() == ref);
}


TEST(test_ex, test1)
{
	std::ofstream file;
	file.open("test5.csv");
	std::string str(
		",A,B,Cell\n1,,0,1\n2,,=A1+B30,0\n30,0,=B1+A1,=B2*10\n"
	);
	file << str;
	file.close();

	CSVReader csv("test5.csv");

	EXPECT_ANY_THROW(csv.getCalcData());
}

TEST(test_ex, test2)
{
	std::ofstream file;
	file.open("test6.csv");
	std::string str(
		",A,B,Cell\n1,=B30+A1,0,1\n2,,=A1+B30,0\n30,0,=B1+A1,=B2*10\n"
	);
	file << str;
	file.close();

	CSVReader csv("test6.csv");
	EXPECT_ANY_THROW(csv.getCalcData());

}

int main(int argc, char* argv[])
{
	CSVReader csv("test1.csv");
	csv.print();

	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}