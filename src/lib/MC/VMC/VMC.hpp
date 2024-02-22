#ifndef __VMC_HPP__
#define __VMC_HPP__
#include "../MC.hpp"
#include "../../system/system.hpp"
#include <iomanip>  //setprecision
#include <iostream> //cerr
#include <fstream>  //ofstream
#include <random>

namespace mcs
{

class VMC : public MC
{
private: 
    static const std::string D_DPROFILE_FILENAME;
    static const std::string D_ENERGY_EVOLUTION_FILENAME;
    static const std::string D_INITCONF_FILENAME;
    static const std::string D_FINCONF_FILENAME;
public:
    VMC(MC_input& input, System * walker)
        :MC(input),_Walker(walker)
    {}

    void run();

    void print_density_profile(const std::string& 
                               filename = D_DPROFILE_FILENAME);
    void print_energy_evolution(const std::string& 
                                filename = D_ENERGY_EVOLUTION_FILENAME);
    void print_inital_configuration(const std::string& 
                                   filename = D_INITCONF_FILENAME );
    void print_final_configuration(const std::string&
                                filename = D_FINCONF_FILENAME);


private:
    System * _Walker;
};

}


#endif 