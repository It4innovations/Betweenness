#include "CsvReader.h"

void CsvReader::readNextRow(std::istream& str)
{
	std::string line;
	std::getline(str, line);
	std::stringstream lineStream(line);
	std::string cell;
	m_data.clear();
	while (std::getline(lineStream, cell, m_separator))
	{
		// Trim newline characters at the end of the line
		cell.erase(std::remove(cell.begin(), cell.end(), '\n'), cell.end());
		cell.erase(std::remove(cell.begin(), cell.end(), '\r'), cell.end());
		cell.erase(std::remove(cell.begin(), cell.end(), ' '), cell.end());

		m_data.push_back(cell);
	}
}

std::size_t CsvReader::size() const
{
	return m_data.size();
}

std::string const& CsvReader::operator[](std::size_t index) const
{
	return m_data[index];
}

std::istream& operator>>(std::istream& str, CsvReader& data)
{
	data.readNextRow(str);
	return str;
}