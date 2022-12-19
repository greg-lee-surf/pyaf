#include "Module/Add/Add_impulses.hpp"
#include "Module/Module.hpp" 

namespace aff3ct
{
namespace module
{

Add_impulses::Add_impulses(int N) : Module()
{
    //setting the module name
    const std::string name="Add_impulses";
    this->set_name(name);
    this->set_short_name(name);
    this->set_single_wave(true);
    this->N = N;

    if(N<=0)    
    {
        std::stringstream message;
        message<<"'N' has to be greater than 0 ('N' = "<<N<<").";
        throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
    }

    //Creating a task 
    auto &t_add = this->create_task("add");

    //Creating the input sockets
    auto s_ix_x     = this->template create_socket_in <int  >(t_add,"ix_x"   ,1);
    auto s_ix_y     = this->template create_socket_in <int  >(t_add,"ix_y"   ,1);
    auto s_delta_x  = this->template create_socket_in <float>(t_add,"delta_x",1);
    auto s_delta_y  = this->template create_socket_in <float>(t_add,"delta_y",1);
    auto s_r_in     = this->template create_socket_in <float>(t_add,"r_in"   ,N);
    auto s_r_out    = this->template create_socket_out<float>(t_add,"r_out"  ,N);

    //Creating codelet

	this->create_codelet(t_add, 
                         [s_ix_x, s_ix_y, s_delta_x, s_delta_y,
                          s_r_in, s_r_out]
                         (Module &m, Task &t, const size_t frame_id) -> int
	{
		static_cast<Add_impulses&>(m).add(static_cast<int*  >(t[s_ix_x   ].get_dataptr()),
                                          	   static_cast<int*  >(t[s_ix_y   ].get_dataptr()),
                                          	   static_cast<float*>(t[s_delta_x].get_dataptr()),
                                       	   static_cast<float*>(t[s_delta_y].get_dataptr()),
                                       	   static_cast<float*>(t[s_r_in   ].get_dataptr()),
                                       	   static_cast<float*>(t[s_r_out  ].get_dataptr()));
		return 0;
	});
}


void Add_impulses::add(int *ix_x, int *ix_y, float *delta_x, float *delta_y, float *r_in, float *r_out)
{
    for(int i = 0 ; i < this->N ; i++)
    {
        r_out[i] = r_in[i];
    }
    r_out[*ix_x] = *delta_x;
    r_out[*ix_y] = *delta_y;
}

}
}
