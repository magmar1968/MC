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
    std::string key;
    if(is_flag_on(_McOpt,McOptions::PrintInitConfiguration)){
        key = "PrintInitConfFilename";
        filename = checkAndGets(_McParam,key);
        _MC->print_inital_configuration(_resultsPath + filename);
    }
    _MC->run();

    if(is_flag_on(_McOpt,McOptions::PrintDensityProfile)){
        key = "PrintDensProfFilename";
        filename = checkAndGets(_McParam,key);
        _MC -> print_density_profile(_resultsPath + filename);
    }
    
    if(is_flag_on(_McOpt,McOptions::PrintEnergyEvolution)){
        key = "PrintEnEvolFilename";
        filename = checkAndGets(_McParam,key);
        _MC -> print_energy_evolution(_resultsPath + filename);
    }

    if(is_flag_on(_McOpt,McOptions::PrintFinalConfiguration)){
        key = "PrintFinalConfFilename";
        filename = checkAndGets(_McParam,key);
        _MC -> print_final_configuration(_resultsPath + filename);
    }
    return _MC->get_results();
}



void Simulation::SetupGas1()
{
    gas_parameters GasParam;
    GasParam.harmonic_lenght = checkAndGetd(_McParam,"HarmonicLenghtGas1");
    GasParam.D               = checkAndGetd(_McParam,"DGas1");
    GasParam.alpha           = checkAndGetd(_McParam,"alphaGas1");
    GasParam.k0              = checkAndGetd(_McParam,"k0Gas1");
    GasParam.Rcore           = checkAndGetd(_McParam,"RcoreGas1");
    GasParam.Rv              = checkAndGetd(_McParam,"RvGas1");
    GasParam.Natoms          = checkAndGeti(_McParam,"NatomsGas1");

    double a_vortex;
    if(is_flag_on(_McOpt,McOptions::AddVortex)){
        a_vortex = checkAndGetd(_McParam,"a_vortex");
        if(is_flag_on(_McOpt, McOptions::SetVortTWFtoOne))
            _Gas1 = new PureGas_HS_Vortex(GasParam,a_vortex,true);
        else    
            _Gas1 = new PureGas_HS_Vortex(GasParam,a_vortex);
    }else{
        _Gas1 = new PureGas_HS(GasParam);
    }

    uint NDensProfStep = checkAndGeti(_McParam,"NDensProfileSteps");
    double DensProfStepSize  = checkAndGeti(_McParam,"DensProfileStepSize");

    _DensProfileGas1 = new mcs::DensProfile(GasParam.Natoms,
                                           NDensProfStep,DensProfStepSize);

}

void Simulation::SetupGas2()
{
    gas_parameters GasParam;
    GasParam.harmonic_lenght = checkAndGetd(_McParam,"HarmonicLenghtGas2");
    GasParam.D               = checkAndGetd(_McParam,"DGas2");
    GasParam.alpha           = checkAndGetd(_McParam,"alphaGas2");
    GasParam.k0              = checkAndGetd(_McParam,"k0Gas2");
    GasParam.Natoms          = checkAndGeti(_McParam,"NatomsGas2");
    GasParam.Rcore           = checkAndGetd(_McParam,"RcoreGas2");
    GasParam.Rv              = checkAndGetd(_McParam,"RvGas2");

    _Gas2 = new PureGas_HS(GasParam);

    uint NDensProfStep       = checkAndGeti(_McParam,"NDensProfileSteps");
    double DensProfStepSize  = checkAndGetd(_McParam,"DensProfileStepSize");
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
    std::string key;

    gas_inter_params IntParams;
    IntParams.alpha = checkAndGetd(_McParam,"alphaGas12");
    IntParams.k0    = checkAndGetd(_McParam,"k0Gas12"   );
    IntParams.Rcore = checkAndGetd(_McParam,"RcoreGas12");
    IntParams.Rv    = checkAndGetd(_McParam,"RvGas12"   );
    IntParams.Natoms_Gas1 = checkAndGeti(_McParam,"NatomsGas1");
    IntParams.Natoms_Gas2 = checkAndGeti(_McParam,"NatomsGas2");
    _Mixture = new Mixture_HS(_Gas1,_Gas2,IntParams);

    if(is_flag_on(_McOpt,McOptions::ReadInitConfFromFile)){
        std::string filename;
        size_t Natoms1 = checkAndGeti(_McParam,"NatomsGas1");
        size_t Natoms2 = checkAndGeti(_McParam,"NatomsGas2");
        key = "InitialConfFilename_1";
        filename = checkAndGets(_McParam,key);
        std::vector<vector_2D> R1 = read_R(filename,Natoms1);
        key = "InitialConfFilename_2";
        filename = checkAndGets(_McParam,key);
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
    McInput.dt          = checkAndGetd(_McParam,"dt"       );
    McInput.N_MCsteps   = checkAndGeti(_McParam,"NMCsteps"   );
    McInput.N_stabsteps = checkAndGeti(_McParam,"NStabSteps" );
    McInput.N_thermsteps= checkAndGeti(_McParam,"NThermSteps");

    _MC = new VMC(McInput,_System);
}

void Simulation::SetupDMC()
{
    size_t N0Walkers;
    MC_input McInput;
    McInput.dt          = checkAndGetd(_McParam,"dt"         );
    McInput.N_MCsteps   = checkAndGeti(_McParam,"NMCsteps"   );
    McInput.N_stabsteps = checkAndGeti(_McParam,"NStabSteps" );
    McInput.N_thermsteps= checkAndGeti(_McParam,"NThermSteps");
    N0Walkers           = checkAndGeti(_McParam,"N0walkers"  );

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

    if(isDirExist(checkAndGets(_McParam,"resultsPath"))){
        _resultsPath = _McParam["resultsPath"];
    }else{     
        if(makePath(_McParam["resultsPath"])){
            _resultsPath = _McParam["resultsPath"];
        }else{
            throw std::invalid_argument("Impossible to create a Results Directory");   
        }
    }
}






}