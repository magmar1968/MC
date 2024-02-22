#include "read_R.hpp"


std::vector<vector_2D> read_R(const std::string& filename)
{
    std::vector<vector_2D> R_OUT;
    if(!file_exist(filename)){
        std::cerr << "ERROR: in _func_   :               \n"
                  << "       the file does't exist       \n";
        return R_OUT;
    }

    std::fstream ifs(filename,std::fstream::in);

    std::vector<std::vector<std::string>> table;
    table = readCSV(ifs);

    vector_2D vec;
    for(uint i_atom = 1; i_atom < table.size(); ++i_atom)
    {
        vec.x = std::stod(table[i_atom][1]);
        vec.y = std::stod(table[i_atom][2]);
        R_OUT.push_back(vec);
    }
    return R_OUT;
}

std::vector<vector_2D> read_R(const std::string& filename, size_t Natoms)
{
    std::vector<vector_2D> R_OUT;
    R_OUT = read_R(filename);

    if(R_OUT.size() < Natoms){
        std::cerr << "ERROR: Inputfile Coordinate to inizialize " << Natoms << std::endl;
    }else{
        R_OUT.resize(Natoms);
    }
    return R_OUT;
}
