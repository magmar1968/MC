#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include <string>

#include "../utility/2D_vector.hpp" //vector 2D

namespace mcs
{


struct Energies{
    double Elocal  = 0;
    double Epot    = 0;
    double Ekin    = 0;
    double EkinFor = 0;

public:
    void operator=(const double scalar)
    {
        this->Elocal  = scalar;
        this->Epot    = scalar;
        this->Ekin    = scalar;
        this->EkinFor = scalar;
    }

    void operator+=(const Energies& e)
    {
        
        this->Elocal = this->Elocal  + e.Elocal;
        this->Epot   = this->Epot    + e.Epot;
        this->Ekin   = this->Ekin    + e.Ekin;
        this->EkinFor= this->EkinFor + e.EkinFor;
    }

    Energies operator/(const double scalar)
    {
        Energies res;
        res.Elocal    = this->Elocal/scalar;
        res.Epot      = this->Epot/scalar;
        res.Ekin      = this->Ekin/scalar;
        res.EkinFor = this->EkinFor/scalar;
        return res;
    }
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
    virtual void store_DensProfile() = 0;

    virtual void print_conf_tofile(const std::string& filename) = 0;
    virtual void print_dens_profile_tofile(const std::string& filename) = 0;

    virtual double Get_NEW_TWF() = 0;
    virtual double Get_OLD_TWF() = 0;

    virtual std::vector<vector_2D> Get_R(uint whichGas = 1) const = 0;
    
    double Get_NEW_EL()const{return _E_NEW.Elocal;};
    double Get_OLD_EL()const{return _E_OLD.Elocal;};

    virtual Energies Get_Energies() const = 0;

    virtual System* copy() const = 0;


protected:
    bool VMC_step_made = false;
    bool DMC_step_made = false; 

    Energies _E_NEW; //new energies
    Energies _E_OLD; //old energies

};

}

#endif 