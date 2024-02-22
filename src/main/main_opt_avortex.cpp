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
    double avortex;
};



int main()
{
    std::string input_filename = "./inputfile/VMCPG_inputfile.dat"; //put inputfile
    std::string output_filename = "./data/out_vortex_opt.csv";
    std::map<std::string,std::string> MC_param;
    McOptions MC_opt = McOptions::Initial;
    read_file(input_filename,MC_param,MC_opt); 
    

    std::vector<double> vortex_params = {125,130,135,140,145,150,155,160};
    std::vector<Results> res;

    for(auto avortex : vortex_params){
        MC_param["a_vortex"] = std::to_string(avortex);
        mcs::Simulation Simulation(MC_param,MC_opt);
        mcs::MC_output ouput = Simulation.Run();

        std::cout << "sym done" << std::endl;
        Results appo;
        appo.avortex = avortex;
        appo.Energy  = ouput.Energy;
        appo.Error   = ouput.Error;
        appo.AccRate = ouput.AcceptRate;
        res.push_back(appo);
    }

    std::fstream ofs(output_filename,std::fstream::out);

    if(ofs.good()){
        ofs << "avortex,Energy,Error,AcceptRate" << std::endl;
        for(auto r : res){
            ofs << r.avortex <<","
                << r.Energy << ","
                << r.Error << ","
                << r.AccRate << std::endl;
        }
    }

    return 0;

}