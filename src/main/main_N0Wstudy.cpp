#include "../lib/simulation/simulation.hpp"
#include "../lib/input/input.hpp"
#include <vector>
#include <string>
#include <fstream>

struct Results
{
    double Energy;
    double Error;
    double AccRate;
    double N0Walkers;
};



int main()
{
    std::string input_filename = "./inputfile/DMC...."; //put inputfile
    std::string output_filename = "./data/E_vs_dt.csv";
    std::map<std::string,std::string> MC_param;
    McOptions MC_opt = McOptions::Initial;

    std::vector<double> N0walkers_arr = {110,100,90,80,70,60};
    std::vector<Results> res;

    for(auto N0walkers : N0walkers_arr){
        MC_param["N0walkers"] = std::to_string(N0walkers);
        mcs::Simulation Simulation(MC_param,MC_opt);
        mcs::MC_output ouput = Simulation.Run();

        Results appo;
        appo.N0Walkers = N0walkers;
        appo.Energy  = ouput.Energy;
        appo.Error   = ouput.Error;
        appo.AccRate = ouput.AcceptRate;
        res.push_back(appo);
    }

    std::fstream ofs(output_filename,std::fstream::out);

    if(ofs.good()){
        ofs << "N0walkers,Energy,Error,AcceptRate" << std::endl;
        for(auto r : res){
            ofs << r.N0Walkers << ","
                << r.Energy    << ","
                << r.Error     << ","
                << r.AccRate   << std::endl;
        }
    }

    return 0;


}