#ifndef ADD_IMPULSES_H
#define ADD_IMPULSES_H

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{

class Add_impulses : public Module
{
    int N;
    public : 
        Add_impulses(int N);
        void add(int *ix_x, int *ix_y, float *delta_x, float *delta_y, float *r_in, float *r_out);
        
};

}
}

#endif 