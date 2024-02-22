#ifndef __PUREGAS_HPP__
#define __PUREGAS_HPP__
#include <vector> //vectors, iterator, uint
#include <random> //std::mt199937,normal_distribution
#include "../utility/2D_vector.hpp"
#include <assert.h> //assert


namespace mcs
{



class PureGas
{
protected:
    using pos_vec_type = std::vector<vector_2D>;
    using cpos_vec_type = const pos_vec_type;
    using force_vec_type = std::vector<vector_2D>;
    
public:
    PureGas(){}
    

    PureGas(size_t Natoms,double h_len, double D)
        :_Natoms(Natoms),_h_len(h_len),_D(D)
    {
        //initialize random device
        std::random_device dev;
        _RNG = std::mt19937(dev());
        _gauss = std::normal_distribution<double>(0,1);
    }

    virtual~PureGas(){};


    virtual pos_vec_type gen_initial_conf()  = 0; 

    virtual double trial_WF(cpos_vec_type&) const = 0;

    //energies
    virtual double Elocal(cpos_vec_type&   ) const = 0;
    virtual double Ekin(cpos_vec_type&     ) const = 0;
    virtual double EkinPartial(cpos_vec_type&) const = 0;
    virtual double EkinFor(cpos_vec_type&  ) const = 0;
    virtual double Epot(cpos_vec_type&     ) const = 0;

    //force
    virtual force_vec_type F(cpos_vec_type&) const = 0;

    //diffuse 
    virtual pos_vec_type diffuse(cpos_vec_type&,double) = 0;

    virtual bool is_configuration_physical(cpos_vec_type&) const = 0;
    
    //getter
    double Get_D()     const {return _D;     }
    double Get_h_len() const {return _h_len; }
    double Get_Natoms() const {return _Natoms;}

    //printing to file
    virtual void print_conf_tofile(const std::string&, cpos_vec_type&) const = 0;
    virtual void print_TWF_tofile(const std::string&) const = 0;


protected:

    uint  _Natoms   = 0;  //Atoms Number
    double _h_len   = 0;
    double _D       = 0;      //Energy scale

    std::mt19937 _RNG;
    std::normal_distribution<double> _gauss;

};


}


#endif