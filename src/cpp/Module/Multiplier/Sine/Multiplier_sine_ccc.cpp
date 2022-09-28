#define _USE_MATH_DEFINES // enable M_PI definition on MSVC compiler
#include <cmath>

// _USE_MATH_DEFINES does not seem to work on MSVC...
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#include "Module/Multiplier/Sine/Multiplier_sine_ccc.hpp"

using namespace aff3ct::module;

template <typename R>
Multiplier_sine_ccc<R>
::Multiplier_sine_ccc(const int N, const R f, const R Fs, const int n_frames)
: Multiplier<R>(N, n_frames), n(0), f(0), nu(0), omega(0), Fs(Fs)
{
	R new_nu = std::floor(f/Fs * (R)1e6) /(R)1e6;
	this->nu = new_nu;
	this->f  = new_nu * this->Fs;
	this->omega = 2 * M_PI * new_nu;
}

template <typename R>
Multiplier_sine_ccc<R>
::~Multiplier_sine_ccc()
{}

template <typename R>
void Multiplier_sine_ccc<R>
::set_f(R f)
{
	this->set_nu(f/Fs);
}

template <typename R>
void Multiplier_sine_ccc<R>
::set_omega(R omega)
{
	this->set_nu(omega / (2 * M_PI));
}

template <typename R>
void Multiplier_sine_ccc<R>
::set_nu(R nu)
{
	R new_nu = std::floor(nu * (R)1e6) /(R)1e6;
	this->nu = new_nu;
	this->f  = new_nu * this->Fs;
	this->omega = 2 * M_PI * new_nu;
}

template <typename R>
R Multiplier_sine_ccc<R>
::get_nu()
{
	return this->nu;
}

template <typename R>
void Multiplier_sine_ccc<R>
::reset()
{
	this->n = (R)0.0;
}

template <typename R>
inline void Multiplier_sine_ccc<R>
::step(const std::complex<R>* x_elt, std::complex<R>* y_elt, const bool inc)
{
	R phase(this->omega * this->n);
	*y_elt = *x_elt * std::complex<R>(std::cos(phase), std::sin(phase));

	this->n = (this->n >= 999999.) ? 0. : this->n + (R)1.;
}

template <typename R>
void Multiplier_sine_ccc<R>
::_imultiply_old(const R *X_N,  R *Z_N, const int frame_id)
{
	// const std::complex<R>* cX_N = reinterpret_cast<const std::complex<R>* >(X_N);
	// std::complex<R>*       cZ_N = reinterpret_cast<      std::complex<R>* >(Z_N);
	// for (auto i = 0 ; i < this->N/2 ; i++)
	// 	this->step(&cX_N[i], &cZ_N[i], false);

	for (auto i = 0 ; i < this->N ; i += 2)
	{
		R phase = this->omega * this->n;

		R cos = std::cos(phase);
		R sin = std::sin(phase);

		Z_N[i +0] = (X_N[i +0] * cos) - (X_N[i +1] * sin);
		Z_N[i +1] = (X_N[i +0] * sin) + (X_N[i +1] * cos);

		this->n = (this->n >= 999999.) ? 0. : this->n + (R)1.;
	}
}

template <typename R>
void Multiplier_sine_ccc<R>
::_imultiply(const R *X_N,  R *Z_N, const int frame_id)
{
	for (auto i = 0; i < mipp::N<R>(); i++)
		this->n_vals[i] = (this->n +(R)i > 999999.) ? (R)i : this->n +(R)i;

	mipp::Reg<R> reg_n = this->n_vals;
	mipp::Reg<R> reg_omega = this->omega;
	mipp::Reg<R> reg_limit = 999999.;
	mipp::Reg<R> reg_limit_plus_one = 1000000.;

	auto end_vec_loop = (this->N / (2 * mipp::N<R>())) * (2 * mipp::N<R>());

	for (auto i = 0 ; i < end_vec_loop; i += 2 * mipp::N<R>())
	{
		auto reg_phase = reg_omega * reg_n;
		auto reg_cos_sin = mipp::cossin(reg_phase);

		mipp::Regx2<R> reg_X_N = &X_N[i];
		reg_X_N = mipp::deinterleave(reg_X_N);

		auto reg_Z_N = reg_X_N * reg_cos_sin;

		reg_Z_N = mipp::interleave(reg_Z_N);
		reg_Z_N.store(&Z_N[i]);

		reg_n += (R)mipp::N<R>();
		reg_n = mipp::blend(reg_n - reg_limit_plus_one, reg_n, reg_n > reg_limit);

		this->n = (this->n + (R)mipp::N<R>() > 999999.) ? this->n + (R)mipp::N<R>() - 1000000. :
		                                                  this->n + (R)mipp::N<R>();
	}

	for (auto i = end_vec_loop ; i < this->N ; i += 2)
	{
		R phase = this->omega * this->n;

		R cos = std::cos(phase);
		R sin = std::sin(phase);

		Z_N[i +0] = (X_N[i +0] * cos) - (X_N[i +1] * sin);
		Z_N[i +1] = (X_N[i +0] * sin) + (X_N[i +1] * cos);

		this->n = (this->n >= 999999.) ? 0. : this->n + (R)1.;
	}
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Multiplier_sine_ccc<float>;
template class aff3ct::module::Multiplier_sine_ccc<double>;
// ==================================================================================== explicit template instantiation
