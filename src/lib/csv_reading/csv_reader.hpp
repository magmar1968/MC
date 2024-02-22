#ifndef __CSV_READER_HPP__
#define __CSV_READER_HPP__

#include <istream>
#include <string>
#include <vector>

enum class CSVState {
    UnquotedField,
    QuotedField,
    QuotedQuote
};

std::vector<std::string> readCSVRow(const std::string &row);
/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::string>> readCSV(std::istream &in);

#endif 