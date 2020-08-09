#include "CSVReader.h"

CSVReader::CSVReader(const std::string & fileName)
{
	loadFile(fileName);
	buildTable();
}

void CSVReader::printWithoutCalc() const
{
	std::cout << initialData;
}

void CSVReader::print()
{
	std::cout << getCalcData();
}

void CSVReader::loadFile(const std::string &fileName)
{
	std::ifstream file(fileName);

	std::copy(
		std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>(), std::back_inserter(initialData)
	);
}

void CSVReader::buildTable()
{
	std::stringstream ssMain(initialData);
	std::getline(ssMain, colsTop, '\n');
	countCols = std::count(colsTop.cbegin(), colsTop.cend(), ',');

	std::string currentTopCell;
	std::stringstream strStrm(colsTop);
	std::getline(strStrm, currentTopCell, ',');
	for (auto i = 0; i < countCols; ++i)
	{
		std::getline(strStrm, currentTopCell, ',');
		col.insert(std::make_pair(currentTopCell, i));
	}

	CellFactory cellFactory;

	size_t i = 0;

	std::string currentStr;
	std::vector<std::unique_ptr<ICell>> currentLine;
	

	while (std::getline(ssMain, currentStr, '\n'))
	{
		std::stringstream strStrm(currentStr);
		std::string strCell;
		std::getline(strStrm, strCell, ',');
		row.insert(std::make_pair(std::stoi(strCell), i++));
		rowsRigth.push_back(strCell);

		auto k = 0;
		currentLine.resize(countCols);

		while (std::getline(strStrm, strCell, ','))
		{
			if (strCell[0] == '=') //if (strCell.find('=') != std::string::npos) 
			{
				currentLine[k++] = std::move(cellFactory.createCell(strCell, this));
			}
			else
			{
				if (!strCell.empty())
					currentLine[k++] = std::move(cellFactory.createCell(std::stoi(strCell)));
				else
					currentLine[k++] = std::move(cellFactory.createCell());
			}
		}
		table.push_back(std::move(currentLine));
	}

	createOpersMap();
}

std::string CSVReader::getCalcData()
{
	setCalcData();
	return calcData;
}

void CSVReader::createOpersMap()
{
	opers.insert(std::make_pair('+', [](long a, long b) {return a + b; }));
	opers.insert(std::make_pair('-', [](long a, long b) {return a - b; }));
	opers.insert(std::make_pair('*', [](long a, long b) {return a * b; }));
	opers.insert(std::make_pair('/', [](long a, long b) {if (b != 0) return a / b; }));
}

void CSVReader::setCalcData()
{
	calcData += colsTop + "\n";
	for (auto j = 0; j < table.size(); ++j)
	{
		calcData +=  rowsRigth[j] + ",";
		for (auto i = 0; i < countCols - 1; ++i)
			calcData += table[j][i]->getStrValue() + ",";

		calcData += table[j][countCols - 1]->getStrValue() + "\n";
	}
}

CSVReader::OperationCell::OperationCell(const std::string & cellStr, CSVReader * csvrPtr)
{
	parse(cellStr);
	csPtr = csvrPtr;
	countInput = 0;

}

long CSVReader::OperationCell::getValue()
{
	if (isCalc)
	{
		return value;
	}
	else
	{
		++countInput;
		if (countInput > 1)
		{
			throw std::runtime_error("The table is not computable");
		}
		std::function<long(long, long)> f = csPtr->opers[oper];
		if (col1 != "" && col2 != "")
		{
			value = f(
				csPtr->table[csPtr->row[row1]][csPtr->col[col1]]->getValue(),
				csPtr->table[csPtr->row[row2]][csPtr->col[col2]]->getValue());
			isCalc = true;
			--countInput;
			return value;
		}
		else
		{
			if (col1 != "" && col2 == "")
			{
				value = f(
					csPtr->table[csPtr->row[row1]][csPtr->col[col1]]->getValue(),
					row2);
				isCalc = true;
				--countInput;
				return value;
			}
			else
			{
				if (col1 == "" && col2 != "")
				{
					value = f(
						row1,
						csPtr->table[csPtr->row[row2]][csPtr->col[col2]]->getValue());
					isCalc = true;
					--countInput;
					return value;
				}
				else
				{
					if (col1 == "" && col2 == "")
					{
						value = f(row1, row2);
						isCalc = true;
						--countInput;
						return value;
					}
				}
			}
		}
	}
}


void CSVReader::OperationCell::parse(const std::string & cellStr)
{
	isCalc = false;

	long i = 1;
	col1 = "";
	col2 = "";
	std::string rowStr1 = "";
	std::string rowStr2 = "";

	while (!isdigit(cellStr[i])) col1.push_back(cellStr[i++]);
	while (isdigit(cellStr[i])) rowStr1.push_back(cellStr[i++]);
	row1 = std::atoi(rowStr1.data());
	oper = cellStr[i++];
	while (!isdigit(cellStr[i])) col2.push_back(cellStr[i++]);
	while (isdigit(cellStr[i])) rowStr2.push_back(cellStr[i++]);
	row2 = std::atoi(rowStr2.data());
}

long CSVReader::EmptyCell::getValue()
{
	throw std::runtime_error("Trying to calculate an empty cell");
	return NULL;
}
