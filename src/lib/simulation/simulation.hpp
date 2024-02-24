
#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include "../input/input.hpp"
#include "../MC/MC.hpp"
#include "../MC/VMC/VMC.hpp"
#include "../MC/DMC/DMC.hpp"

#include "../utility/2D_vector.hpp"
#include "../utility/directory/directory.hpp"

#include "../density_profile/density_profile.hpp"

#include "../puregas/puregas.hpp"
#include "../puregas/puregas_HS/puregas_HS.hpp"
#include "../puregas/puregas_HS/puregas_HS_vortex/puregas_HS_vortex.hpp"

#include "../mixture/mixture_HS.hpp"

#include "../system/system.hpp"
#include "../system/system_puregas/system_puregas.hpp"
#include "../system/system_mixture/system_mixture.hpp"

#include <map> //map
#include <string> //string



namespace mcs
{

class Simulation
{
public:
    Simulation(){};
    Simulation(std::map<std::string,std::string> MC_param, McOptions McOpt)
        :_McParam(MC_param),_McOpt(McOpt)
    {
        SetupSimulation();
    }
    ~Simulation();


    MC_output Run();

private:

    void SetupSimulation();
    void SetupVMC();
    void SetupDMC();
    void SetupGas1();
    void SetupGas2();
    void SetupPureGas();
    void SetupMixture();

private:

    //input
    std::map<std::string,std::string> _McParam;
    McOptions      _McOpt;
    //gasses
    PureGas_HS *   _Gas1;
    PureGas_HS *   _Gas2;
    Mixture_HS *   _Mixture;
    //density profiles
    DensProfile*   _DensProfileGas1;
    DensProfile*   _DensProfileGas2;

    //system
    System *       _System;
    
//pointers
    MC * _MC;

};


}


#endif 