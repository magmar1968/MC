#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include "MC_options.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string.hpp> //trimming
#include <vector>





inline bool file_exist(const std::string& filename)
{
    std::ifstream f(filename.c_str());
    return f.good();
}

bool read_file(const std::string&,  
               std::map<std::string,std::string> &,
               McOptions &);

void read_option( std::string&, std::string &, McOptions &);
void print_sim_parameters(std::map<std::string,std::string>&,
                           McOptions &);



#endif