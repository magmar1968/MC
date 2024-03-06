#ifndef __MC_HPP__
#define __MC_HPP__

#include <string> //string
#include <vector>
#include <iostream>
#include"../system/system.hpp"


namespace mcs{


struct MC_input
{
    uint    N_MCsteps;
    uint    N_thermsteps;
    uint    N_stabsteps;
    double dt;
};

struct MC_output
{
    double Energy =0;
    double Error  =0;
    double AcceptRate =0;
};

class MC{
public:
 public:
    // Constructor and Distructors
    MC(){};
    MC( MC_input& input)
        :_N_MCsteps(input.N_MCsteps),_N_thermsteps(input.N_thermsteps),
        _N_stabsteps(input.N_stabsteps),_dt(input.dt)
    {}
    virtual ~MC(){};

    //Public methods
    void virtual run() = 0;
    
    MC_output get_results()
    {
        MC_output output;
        if(_Had_run == false){
            std::cerr << "ERROR: impossible to return energies result\n"
                  << "       before of running the simulation        \n";
        return output;
        }
        output.Energy = _Energy;
        output.Error  = _Error;
        output.AcceptRate = _AcceptRate;
        return output; 
    }

    //printing methods

    virtual void print_density_profile(const std::string&) = 0;
    virtual void print_energy_evolution(const std::string&) = 0;
    virtual void print_inital_configuration(const std::string&) = 0;
    virtual void print_final_configuration(const std::string&) = 0;


  protected:
    std::vector<Energies> _energy_evolution;
    std::vector<double>   _Eavg_array;
    std::vector<double>   _Error_array;
    int       _N_MCsteps     = 0;
    int       _N_thermsteps  = 0;
    int       _N_stabsteps   = 0;
    double    _dt            = 0;
    double    _Eavg          = 0;
    double    _E2avg         = 0;
    double    _Energy        = 0;
    double    _Error         = 0;
    double    _AcceptRate    = 0;

    bool      _Had_run = false;
};


}







#endif 