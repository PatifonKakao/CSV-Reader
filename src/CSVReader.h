#pragma once

#include <iostream>

#include <fstream>
#include <sstream> 

#include <map>
#include <deque>
#include <vector>

#include <memory>

#include <algorithm>

#include <functional>

#include <stdexcept>





class CSVReader 
{
public:

	CSVReader() {}
	CSVReader(const std::string &fileName);

	void printWithoutCalc() const;
	void print();

	void loadFile(const std::string &fileName);
	void buildTable();

	std::string getCalcData();

private:
	
	class ICell;

	std::string initialData;
	std::string calcData;

	std::deque<std::vector<std::unique_ptr<ICell>>> table;
	std::map<long, long> row;
	std::map<std::string, long> col;

	size_t countCols;
	std::string colsTop;
	std::deque<std::string> rowsRigth;

	std::map<char, std::function<long(long, long)>> opers;
	void createOpersMap();

	void setCalcData();

	
	class ICell
	{
	public:
		virtual long getValue() = 0;
		virtual std::string getStrValue() = 0;
		virtual ~ICell() {}
	};


	class EmptyCell : public ICell
	{
	public:
		virtual long getValue() override;
		virtual std::string getStrValue() override { return ""; }
	};

	class ValueCell : public ICell
	{
	private:
		long value;

	public:

		ValueCell(long value)	{this->value = value;}

		virtual long getValue() override {return value;}
		virtual std::string getStrValue() override { return std::to_string(value); }

	};


	class OperationCell : public ICell
	{
	public:
		OperationCell(const std::string &cellStr, CSVReader * csvrPtr);

		virtual long getValue() override;
		virtual std::string getStrValue() override { return std::to_string(getValue()); }

	private:
		long value;

		std::string col1;
		std::string col2;

		long row1;
		long row2;

		char oper;

		bool isCalc;
		int countInput;

		CSVReader * csPtr;

		void parse(const std::string & cellStr);

	};


	class CellFactory
	{
	public:
		std::unique_ptr<ValueCell> createCell(const long value)
		{
			return std::make_unique<ValueCell>(value);
		}

		std::unique_ptr<OperationCell> createCell(const std::string & cellStr, CSVReader * csvrPrt)
		{
			return std::make_unique<OperationCell>(cellStr, csvrPrt);
		}

		std::unique_ptr<EmptyCell> createCell()
		{
			return std::make_unique<EmptyCell>();
		}

	};
};