#include "VMC.hpp"


namespace mcs
{
const std::string VMC::D_DPROFILE_FILENAME = "./data/VMC_DensProfile.csv";
const std::string VMC::D_ENERGY_EVOLUTION_FILENAME = "./data/VMC_energy_evolution.csv";
const std::string VMC::D_INITCONF_FILENAME = "./data/R_init.csv";
const std::string VMC::D_FINCONF_FILENAME = "./data/R_fin.csv";






void VMC::run()
{
    _Had_run = true;

    //initialize random number generator
    std::random_device dev;
    std::mt19937 RNG = std::mt19937(dev());
    std::uniform_real_distribution<double> unif_distr;
    unif_distr = std::uniform_real_distribution<double>(0.,1.);

    //define all variables
    uint COUNTER = 0,N_accepted_moves = 0;
    bool NO_CROSSING;
    double TWF_OLD, TWF_NEW, diff;
    double c1;
    double E = 0;
    Energies energies;

    energies = _Walker->Get_Energies();

    for(int MC_step = - _N_stabsteps; MC_step <= _N_MCsteps;++MC_step)
    {
        for(int step = 0; step < _N_thermsteps; ++step)
        {
            ++ COUNTER;
            //try VMC STEP
            NO_CROSSING =  _Walker -> try_VMC_step(_dt);
            if(NO_CROSSING)
            {
                TWF_OLD = _Walker -> Get_OLD_TWF();
                TWF_NEW = _Walker -> Get_NEW_TWF();

                diff = 2*(TWF_NEW - TWF_OLD);

                if(diff > 0){
                    _Walker->accept_VMC_step();
                    ++ N_accepted_moves;
                }else{
                    c1 = unif_distr(RNG);
                    if(exp(diff) > c1){
                        _Walker->accept_VMC_step();
                        ++ N_accepted_moves;
                    }
                }
            }
        }
        if(MC_step > 0){
            //update accumulators
            energies = _Walker ->Get_Energies();
            E = energies.Elocal;
            _Eavg  = _Eavg *double(MC_step - 1)/double(MC_step) + E  /double(MC_step);
            _E2avg = _E2avg*double(MC_step - 1)/double(MC_step) + E*E/double(MC_step);
            _Error  = sqrt( _E2avg - _Eavg*_Eavg);

            _Eavg_array.push_back(_Eavg);
            _Error_array.push_back(_Error);
            _energy_evolution.push_back(energies);

            //update density profile
            _Walker->update_DensProfile();

        }
    }

    _Energy = _Eavg;
    _Error  = sqrt( _E2avg - _Eavg*_Eavg);
    _AcceptRate = (100 * N_accepted_moves)/COUNTER; 
    
    return ;
}



//###############################################
//#            Printing Functions               #
//###############################################

void VMC::print_density_profile(const std::string& filename)
{
    if(_Had_run == false)
    {
        std::cerr << "ERROR: impossible to print the density profile \n"
                  << "       before of running the simulation        \n";
        return;
    }

    _Walker->print_dens_profile_tofile(filename);
}

void VMC::print_energy_evolution(const std::string& filename)
{
    if(_Had_run == false)
    {
        std::cerr << "ERROR: impossible to print the energy evolution\n"
                  << "       before of running the simulation        \n";
        return;
    }
    std::fstream  ofs(filename,std::fstream::out);

    if(ofs.bad()){
        std::cerr << "ERROR: unable to print energy evolution" << std::endl;
        return;
    }

    ofs << "MC_STEP,Eavg,Error,ELocal,Epot,Ekin,Ekinfor" << std::endl;
    for(uint step = 0; step < _energy_evolution.size(); ++step)
    {
        Energies E = _energy_evolution[step];
        ofs << step                  << "," << std::setprecision(15)
            << _Eavg_array[step]     << "," 
            << _Error_array[step]    << ","
            << E.Elocal              << ","
            << E.Epot                << ","
            << E.Ekin                << ","
            << E.EkinFor             << std::endl;
    }
}

void VMC::print_inital_configuration(const std::string& filename)
{
    if(_Had_run == true)
    {
        std::cerr << "ERROR: impossible to print the initial configuration \n"
                  << "       after the simulation had been launched        \n";
        return;
    }

    _Walker->print_conf_tofile(filename);
}

void VMC::print_final_configuration(const std::string& filename)
{
    if(_Had_run == false)
    {
        std::cerr << "ERROR: impossible to print the final configuration \n"
                  << "       before the simulation had been launched     \n";
        return;
    }

    _Walker->print_conf_tofile(filename);
}





}