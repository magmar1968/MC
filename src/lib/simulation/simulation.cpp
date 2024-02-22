#include "simulation.hpp"

namespace mcs
{

Simulation::~Simulation()
{
    delete(_MC);
    delete(_System);
    delete(_DensProfileGas1);
    delete(_Gas1);

    if(is_flag_on(_McOpt,McOptions::GasMixture)){
        delete(_DensProfileGas2);
        delete(_Gas2);
    }
}





MC_output Simulation::Run()
{
    std::string filename;
    if(is_flag_on(_McOpt,McOptions::PrintInitConfiguration)){
        filename = _McParam["PrintInitConfFilename"];
        _MC->print_inital_configuration(filename);
    }
    _MC->run();

    if(is_flag_on(_McOpt,McOptions::PrintDensityProfile)){
        filename = _McParam["PrintDensProfFilename"];
        _MC -> print_density_profile(filename);
    }
    
    if(is_flag_on(_McOpt,McOptions::PrintEnergyEvolution)){
        filename = _McParam["PrintEnEvolFilename"];
        _MC -> print_energy_evolution(filename);
    }

    if(is_flag_on(_McOpt,McOptions::PrintFinalConfiguration)){
        filename = _McParam["PrintFinalConfFilename"];
        _MC -> print_final_configuration(filename);
    }
    return _MC->get_results();
}



void Simulation::SetupGas1()
{
    gas_parameters GasParam;
    GasParam.harmonic_lenght = stod(_McParam["HarmonicLenghtGas1"]);
    GasParam.D      = stod(_McParam["DGas1"]);
    GasParam.alpha  = stod(_McParam["alphaGas1"]);
    GasParam.k0     = stod(_McParam["k0Gas1"]);
    GasParam.Natoms = stoi(_McParam["NatomsGas1"]);
    GasParam.Rcore  = stod(_McParam["RcoreGas1"]);
    GasParam.Rv     = stod(_McParam["RvGas1"]);

    double a_vortex;
    if(is_flag_on(_McOpt,McOptions::AddVortex)){
        a_vortex = stod(_McParam["a_vortex"]);
        _Gas1 = new PureGas_HS_Vortex(GasParam,a_vortex);
    }else{
        _Gas1 = new PureGas_HS(GasParam);
    }

    uint NDensProfStep = stoi(_McParam["NDensProfileSteps"]);
    double DensProfStepSize  = stod(_McParam["DensProfileStepSize"]);

    _DensProfileGas1 = new mcs::DensProfile(GasParam.Natoms,
                                           NDensProfStep,DensProfStepSize);

}

void Simulation::SetupGas2()
{
    gas_parameters GasParam;
    GasParam.harmonic_lenght = stod(_McParam["HarmonicLenghtGas2"]);
    GasParam.D      = stod(_McParam["DGas2"]);
    GasParam.alpha  = stod(_McParam["alphaGas2"]);
    GasParam.k0     = stod(_McParam["k0Gas2"]);
    GasParam.Natoms = stoi(_McParam["NatomsGas2"]);
    GasParam.Rcore  = stod(_McParam["RcoreGas2"]);
    GasParam.Rv     = stod(_McParam["RvGas2"]);

    _Gas2 = new PureGas_HS(GasParam);

    uint NDensProfStep = stoi(_McParam["NDensProfileSteps"]);
    double DensProfStepSize  = stod(_McParam["DensProfileStepSize"]);
    _DensProfileGas2 = new mcs::DensProfile(GasParam.Natoms,
                                            NDensProfStep,DensProfStepSize);

}

void Simulation::SetupPureGas()
{
    SetupGas1();

    if(is_flag_on(_McOpt,McOptions::ReadInitConfFromFile)){
        std::string filename;
        filename = _McParam["InitialConfFilename_1"];
        _System = new System_PureGas(_Gas1,_DensProfileGas1,
                                     read_R(filename));
    }else{
        std::vector<vector_2D> R = _Gas1->gen_initial_conf();
        _System = new System_PureGas(_Gas1,_DensProfileGas1,R);
    }
    
}

void Simulation::SetupMixture()
{
    SetupGas1();
    SetupGas2();

    gas_inter_params IntParams;
    IntParams.alpha = stod(_McParam["alphaGas12"]);
    IntParams.k0    = stod(_McParam["k0Gas12"]);
    IntParams.Rcore = stod(_McParam["RcoreGas12"]);
    IntParams.Rv    = stod(_McParam["RvGas12"]);
    IntParams.Natoms_Gas1 = stoi(_McParam["NatomsGas1"]);
    IntParams.Natoms_Gas2 = stoi(_McParam["NatomsGas2"]);
    _Mixture = new Mixture_HS(_Gas1,_Gas2,IntParams);

    if(is_flag_on(_McOpt,McOptions::ReadInitConfFromFile)){
        std::string filename;
        size_t Natoms1 = stoi(_McParam["NatomsGas1"]);
        size_t Natoms2 = stoi(_McParam["NatomsGas2"]);
        filename = _McParam["InitialConfFilename_1"];
        std::vector<vector_2D> R1 = read_R(filename,Natoms1);
        filename = _McParam["InitialConfFilename_2"];
        std::vector<vector_2D> R2 = read_R(filename,Natoms2);
        _System = new System_Mixture(_Mixture,_DensProfileGas1,_DensProfileGas2,
                                      R1,R2);

    }else{
        std::vector<vector_2D> R1 = _Gas1->gen_initial_conf();
        std::vector<vector_2D> R2 = _Gas2->gen_initial_conf();
        _System = new System_Mixture(_Mixture,_DensProfileGas1,_DensProfileGas2,R1,R2);
    }

}

void Simulation::SetupVMC(){
    MC_input McInput;
    McInput.dt          = stod(_McParam["dt"]);
    McInput.N_MCsteps   = stoi(_McParam["NMCsteps"]);
    McInput.N_stabsteps = stoi(_McParam["NStabSteps"]);
    McInput.N_thermsteps= stoi(_McParam["NThermSteps"]);

    _MC = new VMC(McInput,_System);
}

void Simulation::SetupDMC()
{
    size_t N0Walkers;
    MC_input McInput;
    McInput.dt          = stod(_McParam["dt"]);
    McInput.N_MCsteps   = stoi(_McParam["NMCsteps"]);
    McInput.N_stabsteps = stoi(_McParam["NStabSteps"]);
    McInput.N_thermsteps= stoi(_McParam["NThermSteps"]);
    N0Walkers           = stoi(_McParam["N0walkers"]);

    std::vector<mcs::System*> Walkers(N0Walkers);
    for(auto& walker : Walkers){
        walker = _System->copy();
    }
    _MC = new DMC(McInput,Walkers,N0Walkers);
    
}

void Simulation::SetupSimulation()
{
    if(is_flag_on(_McOpt,McOptions::PureGas)){
        SetupPureGas();
    }else{
        SetupMixture();
    }

    if(is_flag_on(_McOpt,McOptions::VMC)){
        SetupVMC();
    }else{
    SetupDMC();
    }
}






}