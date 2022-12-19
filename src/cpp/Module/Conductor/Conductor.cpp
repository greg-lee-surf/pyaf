#include <string>
#include <memory>

#include <vector>
#include <math.h>

#include "Module/Conductor/Conductor.hpp"
#include "Module/Module.hpp" 

namespace aff3ct 
{
namespace module
{

Conductor::Conductor(std::vector<float> _noisy_vec, int _N, int _ix_x, int _ix_y) 
: Module()// , _noisy_vec(_noisy_vec), _n(_n), _ix_x(_ix_x), _ix_y(ix_y)
{
    const std::string name = "Conductor";
    this->set_name(name);
    this->set_short_name(name);
    this->set_single_wave(true);

    this->noisy_vec     = _noisy_vec;
    this->N             = _N;
    this->delta_x_range = arange(-4, 4, 0.05);
    this->delta_y_range = arange(-4, 4, 0.05);
    this->ix_x          = _ix_x;
    this->ix_y          = _ix_y;
    this->x             = 0;
    this->y             = 0;
    this->vec_cnt       = 0;
    this->delta_x       = 0;
    this->delta_y       = 0;

    auto &p1 = this->create_task("generate");

	auto p1s_noisy_vec = this->template create_socket_out<float> (p1, "noisy_vec", this->N  );
	auto p1s_delta_x   = this->template create_socket_out<float> (p1, "delta_x"  , 1        );
	auto p1s_delta_y   = this->template create_socket_out<float> (p1, "delta_y"  , 1        );
	auto p1s_ix_x      = this->template create_socket_out<int>   (p1, "ix_x"     , 1        );
	auto p1s_ix_y      = this->template create_socket_out<int>   (p1, "ix_y"     , 1        );
	auto p1s_x         = this->template create_socket_out<int>   (p1, "x"        , 1        );
	auto p1s_y         = this->template create_socket_out<int>   (p1, "y"        , 1        );


	this->create_codelet(p1, 
                         [p1s_noisy_vec,
                          p1s_delta_x  , p1s_delta_y,
                          p1s_ix_x     , p1s_ix_y   ,
                          p1s_x        , p1s_y       ]
                         (Module &m, Task &t, const size_t frame_id) -> int
	{
		static_cast<Conductor&>(m)._generate(static_cast<float*>(t[p1s_noisy_vec].get_dataptr()),
                                             static_cast<float*>(t[p1s_delta_x].get_dataptr()),
                                        	 static_cast<float*>(t[p1s_delta_y].get_dataptr()),
                                       	     static_cast<int*>  (t[p1s_ix_x].get_dataptr()),
                                       	     static_cast<int*>  (t[p1s_ix_y].get_dataptr()),
                                       	     static_cast<int*>  (t[p1s_x].get_dataptr()),
                                       	     static_cast<int*>  (t[p1s_y].get_dataptr()));

		return 0;
	});
}

void Conductor::_generate(float *_noisy_vec, 
                          float *_delta_x  , float *_delta_y, 
                          int *_ix_x       , int *_ix_y, 
                          int *_x          , int *_y)
{
    if (vec_cnt == 0) { // On initialise le noisy_vec puis on y touche plus
        for (int i = 0 ; i < (int)this->noisy_vec.size() ; i++) {
            _noisy_vec[i] = this->noisy_vec[i];
        }
    }
    *_ix_x      = this->ix_x;
    *_ix_y      = this->ix_y;
    *_x         = (int)floor(this->vec_cnt / this->delta_y_range.size());
    *_y         = this->vec_cnt % (this->delta_y_range.size());
    *_delta_x   = this->delta_x_range[*_x];
    *_delta_y   = this->delta_y_range[*_y];
    this->vec_cnt++;    
}

std::vector<float> Conductor::
arange(float start, float stop, double step)
{
    int n = (stop - start) / step;
    std::vector<float> fill;
    // float result;
    for (int i = 0 ; i < n ; i++)
    {
        // Arrondi à trois chiffres après la virgule... Ne fonctionne pas !!!
        // result = std::floor((start + i * step) * 1000) / 1000; 
        fill.push_back(start + i * step);
    }
    return fill;
}


}
}
