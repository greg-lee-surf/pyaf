#ifndef CONDUCTOR_HPP
#define CONDUCTOR_HPP

#include <vector>
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{

class Conductor : public Module 
{
public: 
    std::vector<float> noisy_vec;
    int N;
    int enable; 
    int ix_x;       // Position dans noisy_vec  
    int ix_y;       // Position dans noisy_vec 
    int x;          // Position dans l'image 
    int y;          // Position dans l'image 
    float delta_x;  // modif. de noisy_vec
    float delta_y;  // modif. de noisy_vec
    std::vector<float> delta_x_range;
    std::vector<float> delta_y_range;
    int vec_cnt;

    Conductor(std::vector<float> _noisy_vec, int _N, int _ix_x, int _ix_y);
//  std::copy(this_>noisy_vec.begin(), this->noisy_vec.end(), _noisy_vec)
    void _generate(float *_noisy_vec, 
             int *_enable, float *_delta_x, 
             float *_delta_y, int *_ix_x,
             int *_ix_y, int *_x, int *_y);

private:
    std::vector<float> arange(float start, float stop, double step); //std::iota
};

}
}
#endif
