//
// BAGEL - Brilliantly Advanced General Electronic Structure Library
// Filename: MSCASPT2.h
// Copyright (C) 2014 Shiozaki group
//
// Author: Shiozaki group <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#ifndef __SRC_SMITH_MSCASPT2_H
#define __SRC_SMITH_MSCASPT2_H

#include <src/smith/caspt2/CASPT2.h>

namespace bagel {
namespace SMITH {
namespace MSCASPT2{

class MSCASPT2 {
  protected:
    std::shared_ptr<const SMITH_Info<double>> info_;

    IndexRange virt_;
    IndexRange active_;
    IndexRange closed_;
    IndexRange ci_;
    std::shared_ptr<const IndexRange> rvirt_;
    std::shared_ptr<const IndexRange> ractive_;
    std::shared_ptr<const IndexRange> rclosed_;
    std::shared_ptr<const IndexRange> rci_;

    std::shared_ptr<const Matrix> heff_;
    std::shared_ptr<const Matrix> fockact_;
    std::shared_ptr<const Matrix> xmsmat_;
    std::vector<double> e0all_;

    // tensors used by SMITH internally
    double e0_;
    std::shared_ptr<Tensor> t2;
    std::shared_ptr<Tensor> l2;
    std::shared_ptr<Tensor> h1_;
    std::shared_ptr<Tensor> f1_;
    std::shared_ptr<Tensor> v2_;
    std::shared_ptr<Tensor> den1;
    std::shared_ptr<Tensor> den2;
    std::shared_ptr<Tensor> Den1;
    std::shared_ptr<Tensor> deci;
    std::shared_ptr<Tensor> rdm0_;
    std::shared_ptr<Tensor> rdm1_;
    std::shared_ptr<Tensor> rdm2_;
    std::shared_ptr<Tensor> rdm3_;
    std::shared_ptr<Tensor> rdm4_;
    std::shared_ptr<Vec<Tensor>> den0ciall;
    std::shared_ptr<Vec<Tensor>> den1ciall;
    std::shared_ptr<Vec<Tensor>> den2ciall;
    std::shared_ptr<Vec<Tensor>> den3ciall;
    std::shared_ptr<Vec<Tensor>> den4ciall;
    std::shared_ptr<Tensor> den0ci;
    std::shared_ptr<Tensor> den1ci;
    std::shared_ptr<Tensor> den2ci;
    std::shared_ptr<Tensor> den3ci;
    std::shared_ptr<Tensor> den4ci;
    std::shared_ptr<Tensor> den0cit;
    std::shared_ptr<Tensor> den1cit;
    std::shared_ptr<Tensor> den2cit;
    std::shared_ptr<Tensor> den3cit;
    std::shared_ptr<Tensor> den4cit;

    // storage for output
    std::shared_ptr<Matrix> den1_;
    std::shared_ptr<Matrix> den2_;
    std::shared_ptr<Tensor> Den1_;
    std::shared_ptr<Dvec> ci_deriv_;
    std::shared_ptr<Matrix> dcheck_;
    // for derivative coupling only
    std::shared_ptr<Matrix> vden1_;

    // passed from CASPT2
    std::vector<std::shared_ptr<MultiTensor>> t2all_;
    std::vector<std::shared_ptr<MultiTensor>> lall_;
    std::vector<std::shared_ptr<MultiTensor>> rall_;
    std::shared_ptr<Vec<Tensor>> rdm0all_;
    std::shared_ptr<Vec<Tensor>> rdm1all_;
    std::shared_ptr<Vec<Tensor>> rdm2all_;
    std::shared_ptr<Vec<Tensor>> rdm3all_;
    std::shared_ptr<Vec<Tensor>> rdm4all_;
    std::shared_ptr<Tensor> rdm0deriv_;
    std::shared_ptr<Tensor> rdm1deriv_;
    std::shared_ptr<Tensor> rdm2deriv_;
    std::shared_ptr<Tensor> rdm3deriv_;
    std::shared_ptr<Tensor> rdm3fderiv_;
    std::shared_ptr<Tensor> rdm4deriv_;

    std::shared_ptr<FutureTensor> Gamma0_();
    std::shared_ptr<FutureTensor> Gamma31_();
    std::shared_ptr<FutureTensor> Gamma34_();
    std::shared_ptr<FutureTensor> Gamma92_();
    std::shared_ptr<FutureTensor> Gamma1_();
    std::shared_ptr<FutureTensor> Gamma32_();
    std::shared_ptr<FutureTensor> Gamma35_();
    std::shared_ptr<FutureTensor> Gamma2_();
    std::shared_ptr<FutureTensor> Gamma37_();
    std::shared_ptr<FutureTensor> Gamma3_();
    std::shared_ptr<FutureTensor> Gamma4_();
    std::shared_ptr<FutureTensor> Gamma56_();
    std::shared_ptr<FutureTensor> Gamma57_();
    std::shared_ptr<FutureTensor> Gamma5_();
    std::shared_ptr<FutureTensor> Gamma58_();
    std::shared_ptr<FutureTensor> Gamma6_();
    std::shared_ptr<FutureTensor> Gamma7_();
    std::shared_ptr<FutureTensor> Gamma60_();
    std::shared_ptr<FutureTensor> Gamma9_();
    std::shared_ptr<FutureTensor> Gamma62_();
    std::shared_ptr<FutureTensor> Gamma12_();
    std::shared_ptr<FutureTensor> Gamma65_();
    std::shared_ptr<FutureTensor> Gamma14_();
    std::shared_ptr<FutureTensor> Gamma16_();
    std::shared_ptr<FutureTensor> Gamma22_();
    std::shared_ptr<FutureTensor> Gamma28_();
    std::shared_ptr<FutureTensor> Gamma29_();
    std::shared_ptr<FutureTensor> Gamma51_();
    std::shared_ptr<FutureTensor> Gamma110_();
    std::shared_ptr<FutureTensor> Gamma111_();
    std::shared_ptr<FutureTensor> Gamma112_();
    std::shared_ptr<FutureTensor> Gamma113_();
    std::shared_ptr<FutureTensor> Gamma114_();
    std::shared_ptr<FutureTensor> Gamma115_();
    std::shared_ptr<FutureTensor> Gamma116_();
    std::shared_ptr<FutureTensor> Gamma117_();
    std::shared_ptr<FutureTensor> Gamma119_();
    std::shared_ptr<FutureTensor> Gamma122_();
    std::shared_ptr<FutureTensor> Gamma124_();
    std::shared_ptr<FutureTensor> Gamma126_();
    std::shared_ptr<FutureTensor> Gamma132_();
    std::shared_ptr<FutureTensor> Gamma138_();
    std::shared_ptr<FutureTensor> Gamma139_();
    std::shared_ptr<FutureTensor> Gamma141_();
    std::shared_ptr<FutureTensor> Gamma142_();
    std::shared_ptr<FutureTensor> Gamma144_();
    std::shared_ptr<FutureTensor> Gamma145_();
    std::shared_ptr<FutureTensor> Gamma147_();
    std::shared_ptr<FutureTensor> Gamma148_();
    std::shared_ptr<FutureTensor> Gamma161_();
    std::shared_ptr<FutureTensor> Gamma166_();
    std::shared_ptr<FutureTensor> Gamma167_();
    std::shared_ptr<FutureTensor> Gamma168_();
    std::shared_ptr<FutureTensor> Gamma169_();
    std::shared_ptr<FutureTensor> Gamma170_();
    std::shared_ptr<FutureTensor> Gamma179_();
    std::shared_ptr<FutureTensor> Gamma191_();
    std::shared_ptr<FutureTensor> Gamma202_();
    std::shared_ptr<FutureTensor> Gamma217_();
    std::shared_ptr<FutureTensor> Gamma239_();

    std::shared_ptr<Queue> make_densityq(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> make_density1q(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> make_density2q(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> make_deciq(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> make_deci2q(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> make_deci3q(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> make_deci4q(const bool reset = true, const bool diagonal = true);
    std::shared_ptr<Queue> contract_rdm_deriv(const std::shared_ptr<const CIWfn> ciwfn, std::shared_ptr<VectorB> bdata, const int offset, const int cisize, const bool reset = true, const bool diagonal = true);
    void zero_total();
    void add_total(double factor);
    void do_rdm_deriv(double factor);

    // same function as that implemented in SpinFreeMethod
    void set_rdm(const int ist, const int jst) {
      rdm0_ = rdm0all_->at(jst, ist);
      rdm1_ = rdm1all_->at(jst, ist);
      rdm2_ = rdm2all_->at(jst, ist);
      rdm3_ = rdm3all_->at(jst, ist);
      rdm4_ = rdm4all_->at(jst, ist);
      mpi__->barrier();
    }

  public:
    MSCASPT2(const CASPT2::CASPT2& cas);
    ~MSCASPT2() {}

    void solve_deriv();
    void solve_nacme();
    void solve_dm();

    std::shared_ptr<const Matrix> rdm11() const { return den1_; }
    std::shared_ptr<const Matrix> rdm12() const { return den2_; }
    std::shared_ptr<const Tensor> rdm21() const { return Den1_; }
    std::shared_ptr<const Matrix> vden1() const { return vden1_; }
    std::shared_ptr<Dvec> ci_deriv() const { return ci_deriv_; }
    std::shared_ptr<const Matrix> dcheck() const { return dcheck_; }
};

}
}
}
#endif

