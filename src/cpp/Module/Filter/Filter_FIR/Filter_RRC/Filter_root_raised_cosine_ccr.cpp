#include <iostream>
#include <complex>
#include <cmath>
#include <limits>

#include "Module/Filter/Filter_FIR/Filter_RRC/Filter_root_raised_cosine_ccr.hpp"
using namespace aff3ct::module;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

template <typename R>
std::vector<R> Filter_root_raised_cosine_ccr<R>
::synthetize(const R rolloff, const int samples_per_symbol, const int delay_in_symbol)
{
	std::vector<R> rrc_coefs (2*delay_in_symbol*samples_per_symbol + 1, R(0));
	R eps = std::numeric_limits<R>::epsilon();

	rrc_coefs[delay_in_symbol*samples_per_symbol] = (R(1.0) - rolloff + R(4.0)*rolloff/(R)M_PI);
	R energie(rrc_coefs[delay_in_symbol*samples_per_symbol] * rrc_coefs[delay_in_symbol*samples_per_symbol]);
	for (int i = 1 ; i < delay_in_symbol*samples_per_symbol+1 ; i++)
	{
		R t = (R)i/(R)samples_per_symbol;
		R value(0);
        if ( std::abs(R(4.0)*rolloff*t - (R)1.0) <=  eps || std::abs(R(4)*rolloff*t + (R)1.0) <= eps )
		{
			value = rolloff/sqrt((R)2.0)*( ((R)1.0+(R)2.0/(R)M_PI)*sin((R)M_PI /((R)4.0*rolloff))+
			                               ((R)1.0-(R)2.0/(R)M_PI)*cos((R)M_PI /((R)4.0*rolloff))
			                             );
		}
        else
        {
            R denom = (R)M_PI * t * ((R)1.0 - (R)16.0*rolloff*rolloff*t*t);
            R numer = sin((R)M_PI * t * ((R)1.0 - rolloff)) + (R)4.0*rolloff*t*cos((R)M_PI * t * ((R)1.0 + rolloff));
            value   = numer/denom;

        }
		rrc_coefs[delay_in_symbol*samples_per_symbol + i] = value;
		rrc_coefs[delay_in_symbol*samples_per_symbol - i] = value;
		energie += value*value + value*value;
	}

	for (size_t i = 0 ; i < rrc_coefs.size() ; i++)
		rrc_coefs[i] /= std::sqrt(energie);

	return rrc_coefs;
}

template <typename R>
Filter_root_raised_cosine_ccr<R>
::Filter_root_raised_cosine_ccr(const int N, const R rolloff, const int samples_per_symbol, const int delay_in_symbol, const int n_frames)
: Filter_FIR_ccr<R>(N, Filter_root_raised_cosine_ccr<R>::synthetize(rolloff, samples_per_symbol, delay_in_symbol), n_frames), rolloff(rolloff), samples_per_symbol(samples_per_symbol), delay_in_symbol(delay_in_symbol)
{
}

template <typename R>
Filter_root_raised_cosine_ccr<R>
::~Filter_root_raised_cosine_ccr(){}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_root_raised_cosine_ccr<float>;
template class aff3ct::module::Filter_root_raised_cosine_ccr<double>;
// ==================================================================================== explicit template instantiation
