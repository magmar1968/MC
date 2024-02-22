#include <iostream>
#include <map>
#include <string>
#include "../lib/puregas/puregas_HS/puregas_HS.hpp"
#include "../lib/mixture/mixture_HS.hpp"
#include "../lib/input/parsing.hpp"
#include "../lib/input/read_R.hpp"
#include "../lib/test_lib/tester.hpp"
#include <assert.h>

#define EPSILON 0.0001

bool AreSame(double ,double);
bool AreSame(std::vector<vector_2D>, std::vector<vector_2D>);

int main()
{
    std::map<std::string,std::string> MC_param;
    McOptions MC_opt = McOptions::Initial;

    std::string input_filename = "./inputfile/example_inputfile.dat";
    std::string R_filename = "./data/init_conf/R1_init.csv";

    read_file(input_filename,MC_param,MC_opt);

    std::vector<vector_2D> R = read_R(R_filename);
    std::vector<vector_2D>::iterator middlepoint = R.begin();
    std::advance(middlepoint,R.size()/2);
    std::vector<vector_2D> R1(R.begin(),middlepoint);
    std::vector<vector_2D> R2(middlepoint,R.end());

    mcs::gas_parameters gas_param;
    gas_param.harmonic_lenght = stod(MC_param["HarmonicLenghtGas1"]);
    gas_param.D      = stod(MC_param["DGas1"]);
    gas_param.alpha  = stod(MC_param["alphaGas1"]);
    gas_param.k0     = stod(MC_param["k0Gas1"]);
    gas_param.Natoms = R1.size();
    gas_param.Rcore  = stod(MC_param["RcoreGas1"]);
    gas_param.Rv     = stod(MC_param["RvGas1"]);


    mcs::PureGas_HS * Gas1 = new mcs::PureGas_HS(gas_param);
    mcs::PureGas_HS * Gas2 = new mcs::PureGas_HS(gas_param);
    gas_param.Natoms = R.size();
    mcs::PureGas_HS * Gas3 = new mcs::PureGas_HS(gas_param);

    mcs::gas_inter_params int_param;
    int_param.alpha = gas_param.alpha;
    int_param.k0 = gas_param.k0;
    int_param.Rcore = gas_param.Rcore;
    int_param.Rv = gas_param.Rv;
    int_param.Natoms_Gas1 = R1.size();
    int_param.Natoms_Gas2 = int_param.Natoms_Gas1;

    mcs::Mixture_HS * Mixture = new mcs::Mixture_HS(Gas1,Gas2,int_param);

    Mixture->print_TWF_tofile("./data/TWF.csv");


    std::vector<vector_2D> FMix,F1,F2,FGas;
    FGas = Gas3->F(R);
    F1 = Mixture->F1(R1,R2);
    F2 = Mixture->F2(R1,R2);
    FMix = F1;
    FMix.insert(FMix.end(),F2.begin(),F2.end());


    assert(AreSame(Gas3->Epot(R),   Mixture->Epot(R1,R2)   ));   //Ok
    assert(AreSame(Gas3->trial_WF(R),Mixture->trial_WF(R1,R2))); //Ok
    assert(AreSame(FGas,FMix                               ));   //Ok 
    assert(AreSame(Gas3->EkinFor(R),Mixture->EkinFor(R1,R2)));   //Ok
    assert(AreSame(Gas3->Ekin(R),   Mixture->Ekin(R1,R2)   )); 


    delete(Gas1);
    delete(Gas2);
    delete(Mixture);

}

bool AreSame(double a, double b)
{
    if(abs(a-b) < EPSILON) return true;
    else return false;
}

bool AreSame(std::vector<vector_2D> a, std::vector<vector_2D> b)
{
    assert(a.size() == b.size());

    for(uint i = 0; i < a.size(); ++i){
        if(AreSame(a[i].x,b[i].x) and AreSame(a[i].y,b[i].y))
            continue;
        else
            return false;
    }
    return true;
}


