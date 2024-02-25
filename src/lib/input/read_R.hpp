#ifndef __READ_R_HPP__
#define __READ_R_HPP__

#include "../utility/2D_vector.hpp"
#include "../csv_reading/csv_reader.hpp"
#include "parsing.hpp"
#include <vector> //vector
#include <string> //string
#include <fstream>
#include <stdexcept>

const std::string DEFAULT_FILENAME = "./data/R.csv";

std::vector<vector_2D> read_R(const std::string& filename);
std::vector<vector_2D> read_R(const std::string& filename,size_t Natoms);



#endif 