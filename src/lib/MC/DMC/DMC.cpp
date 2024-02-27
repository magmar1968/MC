#include "DMC.hpp"

namespace mcs
{
const std::string DMC::D_DPROFILE_FILENAME = "./data/DMC_DensProfile.csv";
const std::string DMC::D_ENERGY_EVOLUTION_FILENAME = "./data/DMC_energy_evolution.csv";
const std::string DMC::D_INITCONF_FILENAME = "./data/R_init.csv";
const std::string DMC::D_FINCONF_FILENAME = "./data/R_fin.csv";
    
DMC::~DMC()
{
    for(auto& walker : _Walkers){
        delete(walker);
    }
}


void DMC::run()
{
    using namespace mytm;
    Timer timer;

    _Had_run = true;

    //initialize random number generator
    std::random_device dev;
    std::mt19937 RNG = std::mt19937(dev());
    std::uniform_real_distribution<double> unif_distr;
    unif_distr = std::uniform_real_distribution<double>(0.,1.);
    double c1; //random variable

    uint COUNTER = 0, N_accepted_moves = 0;
    bool NO_CROSSING;

    
    double E_shift = 0;
    double E = 0;
    double E_acc;
    double  E_NEW,E_OLD;
    Energies energies;
    uint N_sons;

    

    for(int MC_step = - _N_stabsteps; MC_step < _N_MCsteps;++MC_step)
    {
        timer.Tick(); //start timer

        for(int step = 0; step < _N_thermsteps; ++step)
        {
            E_acc = 0.;
            std::vector<System*> Walkers_TMP;
            for(auto &walker: _Walkers)
            {
                COUNTER ++;
                NO_CROSSING = walker->try_DMC_step(_dt);
                if(NO_CROSSING){
                    N_accepted_moves ++;
                    c1 = unif_distr(RNG);
                    E_NEW = walker->Get_NEW_EL();
                    E_OLD = walker->Get_OLD_EL();
                    N_sons = floor( exp(-_dt*((E_NEW + E_OLD)/2. - E_shift)) + c1);
                    walker->accept_DMC_step();
                }else{
                    N_sons = 0;
                }

                if(N_sons == 1){
                    E_acc += E_NEW;
                    Walkers_TMP.push_back(walker); //no copy
                    continue;
                }else if(N_sons == 0){
                    delete(walker); 
                    continue;
                }else{
                    for(uint i_son = 1;  i_son <= N_sons; ++i_son )
                    {
                        E_acc += E_NEW;
                        //perform a deep copy of the parent walker
                        mcs::System * son = walker->copy();
                        Walkers_TMP.push_back(son);
                    }
                    continue;
                }

            }//walkers
            
            //swap vectors
            _Walkers.swap(Walkers_TMP);
            //new walker number
            _NWalkers = _Walkers.size();

            E_shift = E_acc/double(_NWalkers) 
                      - 0.1/_dt * log( double(_NWalkers)/_N0Walkers);
        }//therm step
        timer.Tock();//stop timer

        Energies energies_acc;
        if(MC_step > 0){

            for(auto &walker : _Walkers){
                walker->update_DensProfile();
                energies = walker->Get_Energies();
                
                energies_acc.Elocal += energies.Elocal/double(_NWalkers);
                energies_acc.Epot   += energies.Epot/double(_NWalkers);
                energies_acc.Ekin   += energies.Ekin/double(_NWalkers);
                energies_acc.EkinFor+= energies.EkinFor/double(_NWalkers);

            }

            E = energies_acc.Elocal;
            _Eavg  = _Eavg *double(MC_step - 1)/double(MC_step) 
                       + E /double(MC_step);
            _E2avg = _E2avg*double(MC_step - 1)/double(MC_step) 
                      + E*E/double(MC_step);

            _Error  = sqrt(_E2avg - _Eavg*_Eavg);

            _Eavg_array.push_back(_Eavg);
            _Error_array.push_back(_Error);
            _energy_evolution.push_back(energies_acc);
            
            //print stuff
            Time time = timer.Get_Time();
            std::cout << "MC step: " << MC_step  << " / "  << _N_MCsteps  << std::endl
                      << "E      : " << _Eavg <<   "  +- " << _Error  << std::endl
                      << "pop    : " << _NWalkers << std::endl
                      << "in     : " << time.Mins << ":" << time.Secs << std::endl;
        }
    }//MC steps

    _Energy = _Eavg;
    _Error  = sqrt(_E2avg - _Eavg*_Eavg);
    _AcceptRate = (100 * double(N_accepted_moves))/COUNTER;
}


//###############################################
//#            Printing Functions               #
//###############################################

void DMC::print_density_profile(const std::string& filename)
{
    if(_Had_run == false)
    {
        std::cerr << "ERROR: impossible to print the density profile \n"
                  << "       before running the simulation           \n";
        return;
    }

    //the density profile is shared inbetween walkers, therefore
    // we can make it print from whatever walker
    _Walkers[0]->print_dens_profile_tofile(filename);
}

void DMC::print_energy_evolution(const std::string& filename)
{
    if(_Had_run == false)
    {
        std::cerr << "ERROR: impossible to print the energy evolution\n"
                  << "       before running the simulation           \n";
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

void DMC::print_inital_configuration(const std::string& filename)
{
    if(_Had_run == true)
    {
        std::cerr << "ERROR: impossible to print the initial configuration \n"
                  << "       after the simulation had been launched        \n";
        return;
    }

    _Walkers[0]->print_conf_tofile(filename);

}

void DMC::print_final_configuration(const std::string& filename)
{
    if(_Had_run == false)
    {
        std::cerr << "ERROR: impossible to print the final configuration \n"
                  << "       before the simulation had been launched     \n";
        return;
    }

    _Walkers[0]->print_conf_tofile(filename);
}





} // namespace mcs
