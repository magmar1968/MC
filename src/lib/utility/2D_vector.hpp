#ifndef __2D_vector_H__
#define __2D_vector_H__

#include <math.h>
#include <iostream>
#include <vector>


struct vector_2D
{
  public:
    vector_2D();
    vector_2D(const double,const double);
    ~vector_2D(){};

    double x;
    double y;

    //operator overloading
    vector_2D operator+(const vector_2D &) const;
    vector_2D operator-(const vector_2D &) const;
    
    vector_2D operator*(const double    ) const;
    vector_2D operator/(const double    ) const;

    double    operator*(const vector_2D& ) const;
    double&    operator[](const int index );

    bool     operator==(const vector_2D &) const;
    bool     operator!=(const vector_2D &) const;  

    vector_2D& operator+=(const vector_2D & v2)
    {
        this-> x += v2.x;
        this-> y += v2.y;
        return *this;
    }

    vector_2D& operator-=(const vector_2D & v2)
    {
        this-> x -= v2.x;
        this-> y -= v2.y;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream&, const vector_2D&);
    friend vector_2D operator*(const double&, const vector_2D& );

};
double mod(const vector_2D& v); 

std::vector<vector_2D> operator*(const std::vector<vector_2D>&, const double);
std::vector<vector_2D> operator*(const double, const std::vector<vector_2D>& );
std::vector<vector_2D> operator/(const std::vector<vector_2D>&, const double);
std::vector<vector_2D> operator+(const std::vector<vector_2D>&,const std::vector<vector_2D>&);
std::vector<vector_2D> operator-(const std::vector<vector_2D>&,const std::vector<vector_2D>&);



#endif