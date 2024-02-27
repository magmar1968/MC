#ifndef __DIRECTORY_HPP__
#define __DIRECTORY_HPP__


#include <iostream>
#include <string>
#include <sys/stat.h> // stat
#include <errno.h>    // errno, ENOENT, EEXIST


bool isDirExist(const std::string& path);
bool makePath(const std::string& path);

std::string getRawName(const std::string& path);
std::string getExtension(const std::string & path);


#endif 