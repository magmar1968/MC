#include "parsing.hpp"


bool read_file( const std::string& filename,
                std::map<std::string,std::string> & MC_parameters,
                McOptions & MC_options)
{
    if(file_exist(filename) == false){
        throw std::invalid_argument("ERROR: " + filename + " doesn't exist. Please chek and retry");
    }

    std::ifstream ifs(filename,std::fstream::in);
    std::string * line = new std::string();
    std::string parameter_name, parameter_value;
    std::vector<std::string> splitted_line;
    
    while(ifs.good() and !ifs.eof())
    {
        std::getline(ifs,*line);

        ltrim(*line);
        rtrim(*line);
        

        if(line->size() == 0)
            continue;
        

        //ignore comments
        if(line->find("!") != std::string::npos)
        {
            if(line->rfind("!")== 0)
                continue;
            else{
                line->resize(line->find("!"));
            }
        }
        
        //split line in strings
        std::stringstream ss(*line);
        ss >> parameter_name;
        ss >> parameter_value;

        if(parameter_name.rfind("#") == false){
            read_option(parameter_name,parameter_value,MC_options);
        }else{
           MC_parameters[parameter_name] = parameter_value;
        }
    }

    if( is_flag_on(MC_options,  McOptions::PrintSimulationOptions))
        print_sim_parameters(MC_parameters,MC_options);

    delete(line);
    return true;
}


double checkAndGetd( std::map<std::string,std::string>& map, std::string& keyname)
{
    if(keyExists(map,keyname)){
        return stod(map[keyname]);
    }else{
        throw std::invalid_argument("ERROR: " + keyname + " haven't been found in inputfile");
    }
}

int checkAndGeti(std::map<std::string,std::string>& map, std::string& keyname)
{
    if(keyExists(map,keyname)){
        return stoi(map[keyname]);
    }else{
        throw std::invalid_argument("ERROR: " + keyname + " haven't been found in inputfile");
    }
}

std::string checkAndGets(std::map<std::string,std::string>& map, std::string& keyname)
{
    if(keyExists(map,keyname)){
        return map[keyname];
    }else{
        throw std::invalid_argument("ERROR: " + keyname + " haven't been found in inputfile");
    }
}

void read_option(std::string& parameter_name, std::string& parameter_value, McOptions& MC_options)
{
    if(parameter_value == "false")
        return;

    if(parameter_name == "#DMC")                MC_options = MC_options | McOptions::DMC;
    if(parameter_name == "#VMC")                MC_options = MC_options | McOptions::VMC;
    if(parameter_name == "#PureGas")            MC_options = MC_options | McOptions::PureGas;
    if(parameter_name == "#GasMixture")         MC_options = MC_options | McOptions::GasMixture;
    if(parameter_name == "#AddVortex")          MC_options = MC_options | McOptions::AddVortex;
    if(parameter_name == "#SetVortTWFtoOne"   ) MC_options = MC_options | McOptions::SetVortTWFtoOne;
    if(parameter_name == "#ReadInitConfFromFile") MC_options = MC_options | McOptions::ReadInitConfFromFile;
    if(parameter_name == "#PrintEnergyEvolution") MC_options = MC_options | McOptions::PrintEnergyEvolution;
    if(parameter_name == "#PrintDensityProfile")  MC_options = MC_options | McOptions::PrintDensityProfile;
    if(parameter_name == "#PrintFinalConfiguration") MC_options = MC_options | McOptions::PrintFinalConfiguration;
    if(parameter_name == "#PrintTrialWaveFunction") MC_options = MC_options | McOptions::PrintTrialWaveFunction;
    if(parameter_name == "#PrintSimulationOptions") MC_options = MC_options | McOptions::PrintSimulationOptions;
}  



void print_sim_parameters( std::map<std::string,std::string>& MC_parameters,
                           McOptions & MC_options)

{

    std::cout << "###############################################################" << std::endl;
    if( is_flag_on(MC_options, McOptions::DMC) ){
    std::cout << " Diffusion Montecarlo Simulation using the following parameters" << std::endl;    
    }
    else{
    std::cout << " Variational Montecarlo Simulation using the following parameters" << std::endl;    
    }

    std::cout << "   - Number MC Steps       : " << MC_parameters["NMCsteps"] << std::endl;
    std::cout << "   - Number Therm Steps    : " << MC_parameters["NThermSteps"] << std::endl;
    std::cout << "   - Number Stability Steps: " << MC_parameters["NStabSteps"] << std::endl;
    std::cout << "   - dt                    : " << MC_parameters["dt"] << std::endl;
    
    if( is_flag_on(MC_options, McOptions::DMC) ){
    std::cout << "   - Number Initial walkers: " << MC_parameters["N0walkers"];    
    }

    std::cout << " Gas 1:                      "  << std::endl;
    std::cout << "   - Natoms                 : " << MC_parameters["NatomsGas1"] << std::endl; 
    std::cout << "   - Rv                     : " << MC_parameters["RvGas1"] << std::endl;
    std::cout << "   - R core                 : " << MC_parameters["RcoreGas1"] << std::endl;
    std::cout << "   - k0                     : " << MC_parameters["k0Gas1"] << std::endl;
    std::cout << "   - alpha                  : " << MC_parameters["alphaGas1"] << std::endl;
    std::cout << "   - D                      : " << MC_parameters["DGas1"] << std::endl;
    std::cout << "   - Harmonic lenght Gas    : " << MC_parameters["HarmonicLenghtGas1"] << std::endl;
    if( is_flag_on(MC_options, McOptions::AddVortex)){
        std::cout << "  - Vortex Amplitude        : " << MC_parameters["a_vortex"] << std::endl;
    }
    
    if( is_flag_on(MC_options, McOptions::GasMixture)){
    
    std::cout << " Gas  2:                      " << std::endl;
    std::cout << "   - Natoms                 : " << MC_parameters["NatomsGas2"] << std::endl;
    std::cout << "   - Rv                     : " << MC_parameters["RvGas2"] << std::endl;
    std::cout << "   - R core                 : " << MC_parameters["RcoreGas2"] << std::endl;
    std::cout << "   - k0                     : " << MC_parameters["k0Gas2"] << std::endl;
    std::cout << "   - alpha                  : " << MC_parameters["alphaGas2"] << std::endl;
    std::cout << "   - D                      : " << MC_parameters["DGas2"] << std::endl;
    std::cout << "   - Harmonic lenght Gas    : " << MC_parameters["HarmonicLenghtGas2"] << std::endl;
    
    std::cout << " Gas  12:                     " << std::endl;
    std::cout << "   - Rv                     : " << MC_parameters["RvGas12"] << std::endl;
    std::cout << "   - R core                 : " << MC_parameters["RcoreGas12"] << std::endl;
    std::cout << "   - k0                     : " << MC_parameters["k0Gas12"] << std::endl;
    std::cout << "   - alpha                  : " << MC_parameters["alphaGas12"] << std::endl;
    }// Only for Gas Mixture

    std::cout << "Output Files Filenames:       " << std::endl;
    std::cout << "  - InitialConfFilename_1   : " << MC_parameters["InitialConfFilename_1"] << std::endl;
    if(is_flag_on(MC_options,McOptions::GasMixture)){
        std::cout << "  - InitialConfFilename_2   : " << MC_parameters["InitialConfFilename_2"] << std::endl;
    }
    std::cout << "  - PrintInitConfFilename   : " << MC_parameters["PrintInitConfFilename"] << std::endl; 
    std::cout << "  - PrintDensProfFilename   : " << MC_parameters["PrintDensProfFilename"] << std::endl;
    std::cout << "  - PrintEnEvolFilename     : " << MC_parameters["PrintEnEvolFilename"] << std::endl;
    std::cout << "  - PrintFinalConfFilename  : " << MC_parameters["PrintFinalConfFilename"] << std::endl;

    std::cout << " FLAGS:                       " << std::endl;
    std::cout << "  - VMC                     : " << std::boolalpha << is_flag_on(MC_options,McOptions::VMC) << std::endl;
    std::cout << "  - DMC                     : " << std::boolalpha << is_flag_on(MC_options,McOptions::DMC) << std::endl;
    std::cout << "  - MIXTURE                 : " << std::boolalpha << is_flag_on(MC_options,McOptions::GasMixture) << std::endl;
    std::cout << "  - WITH VORTEX             : " << std::boolalpha << is_flag_on(MC_options,McOptions::AddVortex) << std::endl;
    std::cout << "  - PRINT DENSITY PROFILE   : " << std::boolalpha << is_flag_on(MC_options,McOptions::PrintDensityProfile) << std::endl;
    std::cout << "  - PRINT ENERGY EVOLUTION  : " << std::boolalpha << is_flag_on(MC_options,McOptions::PrintEnergyEvolution) << std::endl;
    std::cout << "  - USE EXTERNAL INITCONF   : " << std::boolalpha << is_flag_on(MC_options,McOptions::ReadInitConfFromFile) << std::endl;
    std::cout << "  - SAVE FINAL CONFIGURATION: " << std::boolalpha << is_flag_on(MC_options,McOptions::PrintFinalConfiguration) << std::endl;
    std::cout << "###############################################################" << std::endl;
}

