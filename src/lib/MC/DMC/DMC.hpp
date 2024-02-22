#ifndef __DMC_HPP__
#define __DMC_HPP__
#include "../MC.hpp"
#include "../../system/system.hpp"
#include "../../utility/timer/timer.hpp"
#include <iomanip>  //setprecision
#include <iostream> //cerr
#include <fstream>  //ofstream
#include <random>
#include <vector>



namespace mcs
{
    
class DMC : public MC
{
private:
    static const std::string D_DPROFILE_FILENAME;
    static const std::string D_ENERGY_EVOLUTION_FILENAME;
    static const std::string D_INITCONF_FILENAME;
    static const std::string D_FINCONF_FILENAME;
public:
    DMC(MC_input& input, std::vector<System *> Walkers, size_t N0Walkers)
        :MC(input),_Walkers(Walkers), _N0Walkers(N0Walkers)
    {}

    ~DMC();

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
    std::vector<System *> _Walkers;
    

    size_t              _N0Walkers;
    size_t              _NWalkers;
    uint                _COUNTER = 0;
    uint                _N_accepted_moves = 0;
    bool                _NO_CROSSING;
};





} // namespace mcs



#endif