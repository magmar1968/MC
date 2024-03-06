#ifndef __CSV_WRITER_HPP__
#define __CSV_WRITER_HPP__

#include <string>  //string
#include <vector>  //vector
#include <fstream> //ofstream
#include <iostream> //cerr

namespace csv
{
    using std::string;
    template<typename T>
    extern void writeCSV(string filename ,string header, std::vector<T> data)
    {
    std::ofstream ofs(filename, std::ofstream::out);

    if(ofs.bad())
        std::cerr << "ERROR: in " << __PRETTY_FUNCTION__ << " unable to create: " << filename << std::endl; 
    else{
        ofs << header << std::endl;

        for(auto element : data){
            ofs << element << std::endl;
        }
    }
} 

};



#endif 