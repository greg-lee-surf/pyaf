#include "Module/Filter/Filter_UPFIR/Filter_UPFIR_ccr.hpp"

using namespace aff3ct::module;

template <typename R>
Filter_UPFIR_ccr<R>
::Filter_UPFIR_ccr(const int N, std::vector<R> H, const int F, const int n_frames)
: Filter<R>(N, F * N, n_frames), F(F), H(F), flt_bank()
{
	this->H.resize(F);
	for(size_t i=0; i < H.size(); i++)
		this->H[i % F].push_back(H[i]);

	for (auto f=0;f<F; f++)
		this->flt_bank.push_back(Filter_FIR_ccr<R>(N,this->H[f]));
}

template <typename R>
Filter_UPFIR_ccr<R>
::~Filter_UPFIR_ccr()
{
}

template <typename R>
void Filter_UPFIR_ccr<R>
::step (const std::complex<R>* x_elt, std::complex<R>* y_F)
{
	for (auto f = 0; f<this->F; f++)
		this->flt_bank[f].step(x_elt, y_F + f);
}

template <typename R>
void Filter_UPFIR_ccr<R>
::reset()
{
	for (auto f = 0; f<this->F; f++)
		this->flt_bank[f].reset();
}

// template <typename R>
// void Filter_UPFIR_ccr<R>
// ::_filter(const R *X_N1, R *Y_N2, const int frame_id)
// {
// 	auto cX_N1 = reinterpret_cast<const std::complex<R>* >(X_N1);
// 	auto cY_N2 = reinterpret_cast<std::complex<R>* >(Y_N2);

// 	for (auto f = 0; f<this->F; f++)
// 		for (auto i = 0; i < this->N/2; i++)
// 			this->flt_bank[f].step(cX_N1 + i, cY_N2 + i*this->F + f);
// }

template <typename R>
void Filter_UPFIR_ccr<R>
::_filter(const R *X_N1, R *Y_N2, const int frame_id)
{
    auto cY_N2 = reinterpret_cast<std::complex<R>* >(Y_N2);
    std::vector<R> Y_N1 (this->get_N(), (R)0);
    auto cY_N1 = reinterpret_cast<std::complex<R>* >(Y_N1.data());
    for (int f = 0; f<this->F; f++)
    {
        this->flt_bank[f].filter(X_N1, Y_N1.data());

        for (auto i = 0; i < this->N/2; i++)
            cY_N2[i*this->F + f] = cY_N1[i];
    }
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Filter_UPFIR_ccr<float>;
template class aff3ct::module::Filter_UPFIR_ccr<double>;
// ==================================================================================== explicit template instantiation
