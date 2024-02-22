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
    double dt;
};



int main()
{
    std::string input_filename = "./inputfile/DMC...."; //put inputfile
    std::string output_filename = "./data/E_vs_dt.csv";
    std::map<std::string,std::string> MC_param;
    McOptions MC_opt = McOptions::Initial;

    std::vector<double> dts = {0.005,0.01,0.02,0.05,0.1};
    std::vector<Results> res;

    for(auto dt : dts){
        MC_param["dt"] = std::to_string(dt);
        mcs::Simulation Simulation(MC_param,MC_opt);
        mcs::MC_output ouput = Simulation.Run();

        Results appo;
        appo.dt      = dt;
        appo.Energy  = ouput.Energy;
        appo.Error   = ouput.Error;
        appo.AccRate = ouput.AcceptRate;
        res.push_back(appo);
    }

    std::fstream ofs(output_filename,std::fstream::out);

    if(ofs.good()){
        ofs << "dt,Energy,Error,AcceptRate" << std::endl;
        for(auto r : res){
            ofs << r.dt <<","
                << r.Energy << ","
                << r.Error << ","
                << r.AccRate << std::endl;
        }
    }

    return 0;

}