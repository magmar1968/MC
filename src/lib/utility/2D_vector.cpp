#include "2D_vector.hpp"



vector_2D::vector_2D()
    :x(0.),y(0.)
{}

vector_2D::vector_2D(const double _x, const double _y)
    :x(_x),y(_y)
{}


vector_2D vector_2D::operator+(const vector_2D & v2) const
{
    vector_2D v_res;
    v_res.x = this-> x + v2.x;
    v_res.y = this-> y + v2.y;
    return v_res; 
}

vector_2D vector_2D::operator-(const vector_2D & v2) const
{
    vector_2D v_res;
    v_res.x = this-> x - v2.x;
    v_res.y = this-> y - v2.y;
    return v_res; 
}

vector_2D vector_2D::operator*(const double scalar) const
{
    vector_2D v_res(this-> x * scalar,
                    this-> y * scalar);
    return v_res;
}

vector_2D vector_2D::operator/(const double scalar) const
{
    vector_2D v_res(this-> x / scalar,
                    this-> y / scalar);
    return v_res;
}

double vector_2D::operator*(const vector_2D & v2) const 
{
    return this-> x *v2.x + this-> y * v2.y;
}

double& vector_2D::operator[](const int index) 
{
    if(index == 0)
        return this-> x;
    else if (index == 1)
        return this ->y;
    else{
        std::cerr << "Error in __PRETTY_FUNCTION__ index out of range"  << std::endl;
        return this -> x;
    }
}

bool vector_2D::operator==(const vector_2D& v2) const
{
    if(this-> x == v2.x and this-> y == v2.y )
        return true;
    else 
        return false;
}

bool vector_2D::operator!=(const vector_2D& v2) const
{
    return !(*this == v2);
}

std::ostream& operator<<(std::ostream& os, const vector_2D& v)
{
    os << v.x << " " << v.y;
    return os;
}

vector_2D operator*(const double& scalar, const vector_2D& v)
{
    return v*scalar;
}




/// @brief Compute the modulous of a 2D array
/// @param v  : input array
/// @return the modolous
double mod(const vector_2D& v) 
{
    return std::sqrt(v*v);
}

std::vector<vector_2D> operator*(const std::vector<vector_2D>& v, const double scalar)
{
    std::vector<vector_2D> OUT(v.size());
    for(uint i = 0; i < v.size(); ++i){
        OUT[i] = v[i]*scalar;
    }   
    return OUT;
}

std::vector<vector_2D> operator/(const std::vector<vector_2D>& v, const double scalar)
{
    std::vector<vector_2D> OUT(v.size());
    for(uint i = 0; i < v.size(); ++i){
        OUT[i] = v[i]/scalar;
    }   
    return OUT;
}

std::vector<vector_2D> operator*(const double scalar, const std::vector<vector_2D>& v )
{
    return v*scalar;
}


std::vector<vector_2D> operator+(const std::vector<vector_2D>& v, const std::vector<vector_2D>& w)
{
    std::vector<vector_2D> OUT(v.size());
    for(uint i = 0; i < v.size(); ++ i)
    {
        OUT[i] = v[i] + w[i];
    }
    return OUT;
}

std::vector<vector_2D> operator-(const std::vector<vector_2D>& v, const std::vector<vector_2D>& w)
{
    std::vector<vector_2D> OUT(v.size());
    for(uint i = 0; i < v.size(); ++ i)
    {
        OUT[i] = v[i] - w[i];
    }
    return OUT;
}



