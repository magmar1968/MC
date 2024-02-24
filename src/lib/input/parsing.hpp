#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include "MC_options.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}





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