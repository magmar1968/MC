

#include "../lib/input/parsing.hpp"
#include <map>
#include <bitset>
#include <string>

#include "../lib/test_lib/tester.hpp"


int main()
{

    McOptions MC_opt = McOptions::Initial;
    std::map<std::string,std::string> MC_param;

    TEST("file exist",file_exist("./inputfile/example_inputfile.dat"));

    //TEST PARSING
    read_file("./inputfile/example_inputfile.dat",
                    MC_param,MC_opt);

    TEST("DMC flag",         is_flag_on(MC_opt, McOptions::DMC                    ));
    TEST("VMC flag",         is_flag_off(MC_opt,McOptions::VMC                    ));
    TEST("PureGas flag",     is_flag_off(MC_opt,McOptions::PureGas                ));
    TEST("GasMixture flag",  is_flag_on(MC_opt, McOptions::GasMixture             ));
    TEST("Vortex flag",      is_flag_on(MC_opt, McOptions::AddVortex              ));
    TEST("Energy Evol flag", is_flag_on(MC_opt, McOptions::PrintEnergyEvolution   ));
    TEST("DensProfile flag", is_flag_on(MC_opt, McOptions::PrintDensityProfile    ));
    TEST("Final conf flag",  is_flag_on(MC_opt, McOptions::PrintFinalConfiguration));
    TEST("TWF flag",         is_flag_on(MC_opt, McOptions::PrintTrialWaveFunction ));
    TEST("Sim Option flag",  is_flag_off(MC_opt,McOptions::PrintSimulationOptions ));
    


    TEST("NMCsteps",MC_param["NMCsteps"]    == "100"   ? 1 : 0);
    TEST("NThermSteps",MC_param["NThermSteps"] == "10"   ? 1 : 0);
    TEST("NStabSteps",MC_param["NStabSteps"]  == "100"  ? 1 : 0);
    TEST("dt",MC_param["dt"]          == "0.01" ? 1 : 0);
    TEST("N0walkers",MC_param["N0walkers"]   == "100"  ? 1 : 0);
    TEST("RvGas1",MC_param["RvGas1"]      == "27"   ? 1 : 0);
    TEST("RcoreGas1",MC_param["RcoreGas1"]   == "1"    ? 1 : 0);
    TEST("k0Gas1",MC_param["k0Gas1"]      == "0.001"? 1 : 0);
    TEST("alphaGas1",MC_param["alphaGas1"]   == "10"   ? 1 : 0);
    TEST("DGas1",MC_param["DGas1"]       == "1"   ? 1 : 0);
    TEST("HarmonicLenghtGas1",MC_param["HarmonicLenghtGas1"] == "50" ? 1 : 0);
    TEST("RvGas2",MC_param["RvGas2"]      == "27"   ? 1 : 0);
    TEST("RcoreGas2",MC_param["RcoreGas2"]   == "1"    ? 1 : 0);
    TEST("k0Gas2",MC_param["k0Gas2"]      == "0.001"? 1 : 0);
    TEST("alphaGas2",MC_param["alphaGas2"]   == "10"   ? 1 : 0);
    TEST("DGas2",MC_param["DGas2"]       == "1"   ? 1 : 0);
    TEST("HarmonicLenghtGas2",MC_param["HarmonicLenghtGas2"] == "50" ? 1 : 0);
    TEST("RvGas12",MC_param["RvGas12"]     == "27"   ? 1 : 0);
    TEST("RcoreGas12",MC_param["RcoreGas12"]  == "1"    ? 1 : 0);
    TEST("k0Gas12",MC_param["k0Gas12"]     == "0.001"? 1 : 0);
    TEST("alphaGas12",MC_param["alphaGas12"]  == "10"   ? 1 : 0);
    
    
    

    return 0;

}
