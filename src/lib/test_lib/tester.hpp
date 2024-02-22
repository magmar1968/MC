#ifndef __TESTER_HPP__
#define __TESTER_HPP__

#include <string>
#include <iostream>

#define PASSED true
#define FAILED false


void TEST(std::string testname,bool test)
{
    if(test == FAILED){
        std::cerr << "TEST: " << testname  << "   Failed" <<  std::endl;
    }
}




#endif