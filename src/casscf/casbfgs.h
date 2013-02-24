//
// BAGEL - Parallel electron correlation program.
// Filename: superci.h
// Copyright (C) 2011 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// The BAGEL package is free software; you can redistribute it and\/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// The BAGEL package is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the BAGEL package; see COPYING.  If not, write to
// the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//


#ifndef __NEWINT_CASSCF_CASBFGS_H
#define __NEWINT_CASSCF_CASBFGS_H

#include <memory>
#include <string>
#include <map>
#include <src/scf/scf.h>
#include <src/casscf/casscf.h>
#include <src/wfn/rdm.h>

namespace bagel {

class CASBFGS : public CASSCF {

  protected:
    void common_init() {
      std::cout << "    * Using the Quasi 2nd-order algorithm as noted in Chaban et al. TCA (1997)" << std::endl;
    }

    void grad_vc(std::shared_ptr<const Matrix> cfock, std::shared_ptr<const Matrix> afock, std::shared_ptr<RotFile> sigma) const;
    void grad_va(std::shared_ptr<const Matrix> cfock, std::shared_ptr<const Matrix> qxr,   std::shared_ptr<RotFile> sigma) const;
    void grad_ca(std::shared_ptr<const Matrix> cfock, std::shared_ptr<const Matrix> afock, std::shared_ptr<const Matrix> qxr, std::shared_ptr<RotFile> sigma) const;
#if 0
    void sigma_at_at_(const std::shared_ptr<RotFile> cc, std::shared_ptr<RotFile> sigma,
                      const std::shared_ptr<Matrix> gaa, const std::shared_ptr<Matrix> f);
    void sigma_ai_ai_(const std::shared_ptr<RotFile> cc, std::shared_ptr<RotFile> sigma, const std::shared_ptr<Matrix> f);
    void sigma_at_ai_(const std::shared_ptr<RotFile> cc, std::shared_ptr<RotFile> sigma, const std::shared_ptr<Matrix> fact);
    void sigma_ai_ti_(const std::shared_ptr<RotFile> cc, std::shared_ptr<RotFile> sigma, const std::shared_ptr<Matrix> fact);
    void sigma_ti_ti_(const std::shared_ptr<RotFile> cc, std::shared_ptr<RotFile> sigma,
                      const std::shared_ptr<Matrix> gaa, const std::shared_ptr<Matrix> f, const std::shared_ptr<Matrix> factp);
#endif

#if 0
    void update_orbitals(std::shared_ptr<RotFile> rot);
    std::shared_ptr<Matrix> tailor_rotation(const std::shared_ptr<Matrix> seed);
#endif

  public:
    CASBFGS(const std::multimap<std::string, std::string> idat, const std::shared_ptr<const Geometry> geom)
      : CASSCF(idat, geom) { common_init(); }

    void compute();

};

}

#endif