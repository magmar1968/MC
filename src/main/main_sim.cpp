#include "../lib/simulation/simulation.hpp"
#include "../lib/input/input.hpp"
#include <string>


int main()
{

    std::string input_filename = "./inputfile/DMCPG_inputfile.dat";
    std::map<std::string,std::string> MC_param;
    McOptions MC_opt = McOptions::Initial;
    read_file(input_filename,MC_param,MC_opt); 

    mcs::Simulation Simulation(MC_param,MC_opt);
    mcs::MC_output ouput = Simulation.Run();

    std::cout << "Simulation results: " << std::endl
              << " Energy:  " << ouput.Energy << std::endl
              << " Error:   " << ouput.Error  << std::endl
              << " AccRate: " << ouput.AcceptRate << std::endl;

    return 0;
}