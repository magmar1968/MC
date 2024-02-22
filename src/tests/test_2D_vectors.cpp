#include "../lib/utility/2D_vector.hpp"
#include <vector>


int main(){

    vector_2D v(2,3),w(5,-2),z(-2,-6),k(4,3);
    vector_2D v2(v);
    double scalar = 2;
    bool test_opers;
    bool test_opers_vec;

    test_opers = true;
    test_opers &= v + w == vector_2D(7,1) ? 1 : 0;
    test_opers &= v - w == vector_2D(-3,5)? 1 : 0;
    test_opers &= v * w == 4              ? 1 : 0;
    test_opers &= v * v == 13             ? 1 : 0;
    test_opers &= v * scalar == vector_2D(4,6) ? 1 : 0;
    test_opers &= z/2 == vector_2D(-1,-3) ? 1 : 0; 
    test_opers &= v[0] == 2               ? 1 : 0;
    test_opers &= v == v2                 ? 1 : 0;
    test_opers &= v != w                  ? 1 : 0;
    z += v;
    test_opers &= z == vector_2D(0,-3)    ? 1 : 0; 
    test_opers &= mod(k) == 5             ? 1 : 0;


    if(test_opers == true){
        std::cout << "TEST: vector 2D operations: Ok" << std::endl;
    } else{
        std::cout << "TEST: vector 2D operations: Failed" << std::endl;
    }


    z = vector_2D(2,6);
    uint dim = 10;
    std::vector<vector_2D> vecv(dim,v);
    std::vector<vector_2D> vecw(dim,w);
    std::vector<vector_2D> vecz(dim,z);
    
    test_opers_vec = true;
    for( uint i = 0; i < vecv.size(); ++i){
        test_opers_vec &= (vecv + vecw)[i] == vector_2D(7,1) ? 1 : 0;
        test_opers_vec &= (vecv - vecw)[i] == vector_2D(-3,5)? 1 : 0;
        test_opers_vec &= (vecv*scalar)[i] == vector_2D(4,6) ? 1 : 0;
        test_opers_vec &= (scalar*vecv)[i] == vector_2D(4,6) ? 1 : 0; 
        test_opers_vec &= (vecz / 2.  )[i] == vector_2D(1,3) ? 1 : 0;
    }

    if(test_opers_vec == true){
        std::cout << "TEST: vector of vector2D operations: Ok" << std::endl;
    } else{
        std::cout << "TEST: vector of vector2D operations: Failed" << std::endl;
    }

    if(test_opers and test_opers_vec)
        return 0;
    else 
        return -1;

}