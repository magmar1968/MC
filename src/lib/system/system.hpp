#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include <string>

namespace mcs
{


struct Energies{
    double Elocal  = 0;
    double Epot    = 0;
    double Ekin    = 0;
    double EkinFor = 0;
};


class System
{
public:
    System(){};
    virtual~System(){};

    virtual bool try_VMC_step(double) = 0;
    virtual bool try_DMC_step(double) = 0;

    virtual void accept_VMC_step() = 0;
    virtual void accept_DMC_step() = 0;

    virtual void update_DensProfile() = 0;

    virtual void print_conf_tofile(const std::string& filename) = 0;
    virtual void print_dens_profile_tofile(const std::string& filename) = 0;

    virtual double Get_NEW_TWF() = 0;
    virtual double Get_OLD_TWF() = 0;

    double Get_NEW_EL()const{return NEW_EL;};
    double Get_OLD_EL()const{return OLD_EL;};

    virtual Energies Get_Energies() const = 0;

    virtual System* copy() const = 0;


protected:
    bool VMC_step_made = false;
    bool DMC_step_made = false; 

    double NEW_EL = 0;
    double OLD_EL = 0;



};

}

#endif 