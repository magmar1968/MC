
#include "directory.hpp"

bool isDirExist(const std::string& path)
{    
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool makePath(const std::string& path)
{
    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
            size_t pos = path.find_last_of('/');
            if (pos == std::string::npos)
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
        // now, try to create again
        return 0 == mkdir(path.c_str(), mode);
    case EEXIST:
        // done!
        return isDirExist(path);
    default:
        return false;
    }
}


std::string getRawName(const std::string& path )
{
    size_t last_index = path.find_last_of(".");

    if(last_index == std::string::npos){
        std::cerr << "ERROR: in " << __PRETTY_FUNCTION__ << std::endl
                 <<  "       seems like the input path is empty\n";
        return std::string();
    }
    else
        return path.substr(0,last_index);
}

std::string getExtension(const std::string& path)
{
    size_t last_index = path.find_last_of(".");
    if(last_index == std::string::npos){
        std::cerr << "ERROR: in " << __PRETTY_FUNCTION__ << std::endl
                 <<  "       seems like the input path is empty\n";
        return std::string();
    }
    else
        return path.substr(last_index,path.size());
}