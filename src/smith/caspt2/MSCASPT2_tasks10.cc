//
// BAGEL - Brilliantly Advanced General Electronic Structure Library
// Filename: MSCASPT2_tasks10.cc
// Copyright (C) 2014 Toru Shiozaki
//
// Author: Toru Shiozaki <shiozaki@northwestern.edu>
// Maintainer: Shiozaki group
//
// This file is part of the BAGEL package.
//
// This program is free software: you can redistribute it and/or modify
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

#include <bagel_config.h>
#ifdef COMPILE_SMITH

#include <src/smith/caspt2/MSCASPT2_tasks10.h>

using namespace std;
using namespace bagel;
using namespace bagel::SMITH;
using namespace bagel::SMITH::MSCASPT2;

void Task450::Task_local::compute() {
  const Index x1 = b(0);
  const Index a3 = b(1);
  const Index a1 = b(2);
  const Index c2 = b(3);
  // tensor label: I661
  std::unique_ptr<double[]> odata(new double[out()->get_size(x1, a3, a1, c2)]);
  std::fill_n(odata.get(), out()->get_size(x1, a3, a1, c2), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x1, a3, a1, c2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x1, a3, a1, c2), 0.0);
  for (auto& a4 : *range_[2]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(a4, a3);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a4, a3)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a4.size(), a3.size());
    // tensor label: I678
    std::unique_ptr<double[]> i1data = in(1)->get_block(x1, a4, c2, a1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x1, a4, c2, a1)]);
    sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, x1.size(), a4.size(), c2.size(), a1.size());
    dgemm_("T", "N", a3.size(), x1.size()*c2.size()*a1.size(), a4.size(),
           1.0, i0data_sorted, a4.size(), i1data_sorted, a4.size(),
           1.0, odata_sorted, a3.size());
  }
  sort_indices<1,0,3,2,1,1,1,1>(odata_sorted, odata, a3.size(), x1.size(), c2.size(), a1.size());
  out()->add_block(odata, x1, a3, a1, c2);
}

void Task451::Task_local::compute() {
  const Index x1 = b(0);
  const Index a4 = b(1);
  const Index c2 = b(2);
  const Index a1 = b(3);
  // tensor label: I678
  std::unique_ptr<double[]> odata(new double[out()->get_size(x1, a4, c2, a1)]);
  std::fill_n(odata.get(), out()->get_size(x1, a4, c2, a1), 0.0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x1, a4, c2, a1);
    sort_indices<0,1,2,3,1,1,-1,1>(i0data, odata, x1.size(), a4.size(), c2.size(), a1.size());
  }
  {
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(0)->get_block(x1, a1, c2, a4);
    sort_indices<0,3,2,1,1,1,2,1>(i1data, odata, x1.size(), a1.size(), c2.size(), a4.size());
  }
  out()->add_block(odata, x1, a4, c2, a1);
}

void Task452::Task_local::compute() {
  const Index x5 = b(0);
  const Index x2 = b(1);
  const Index x4 = b(2);
  const Index x3 = b(3);
  const Index x1 = b(4);
  const Index x0 = b(5);
  // tensor label: I521
  std::unique_ptr<double[]> i0data = in(0)->get_block(x1, x0, x2, x5, x4, x3);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x1, x0, x2, x5, x4, x3)]);
  sort_indices<3,2,4,5,0,1,0,1,1,1>(i0data, i0data_sorted, x1.size(), x0.size(), x2.size(), x5.size(), x4.size(), x3.size());
  // tensor label (calculated on-the-fly): Gamma161
  {
    if (x1 == x2) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x0, x4, x3)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x0, x4, x3), 0.0);
      for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
        for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
          for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
            for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix0+x0.size()*(ix4+x4.size()*(ix3)))] +=
                  1.0 * i0data_sorted[ix5+x5.size()*(ix2+x2.size()*(ix4+x4.size()*(ix3+x3.size()*(ix2+x1.size()*(ix0)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x0, x4, x3);
    }
  }
  {
    if (x1 == x3) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x2, x4, x0)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x2, x4, x0), 0.0);
      for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
        for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
          for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
            for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix2+x2.size()*(ix4+x4.size()*(ix0)))] +=
                  1.0 * i0data_sorted[ix5+x5.size()*(ix2+x2.size()*(ix4+x4.size()*(ix3+x3.size()*(ix3+x1.size()*(ix0)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x2, x4, x0);
    }
  }
  {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x5, x2, x4, x3, x1, x0)]);
    std::fill_n(o3data.get(), out(3)->get_size(x5, x2, x4, x3, x1, x0), 0.0);
    sort_indices<0,1,2,3,4,5,1,1,1,1>(i0data_sorted, o3data, x5.size(), x2.size(), x4.size(), x3.size(), x1.size(), x0.size());
    out(3)->add_block(o3data, x5, x2, x4, x3, x1, x0);
  }
}

void Task453::Task_local::compute() {
  const Index x1 = b(0);
  const Index x0 = b(1);
  const Index x2 = b(2);
  const Index x5 = b(3);
  const Index x4 = b(4);
  const Index x3 = b(5);
  // tensor label: I521
  std::unique_ptr<double[]> odata(new double[out()->get_size(x1, x0, x2, x5, x4, x3)]);
  std::fill_n(odata.get(), out()->get_size(x1, x0, x2, x5, x4, x3), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x1, x0, x2, x5, x4, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x1, x0, x2, x5, x4, x3), 0.0);
  for (auto& a2 : *range_[2]) {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x5, a2, x4, x3);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, a2, x4, x3)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x5.size(), a2.size(), x4.size(), x3.size());
    // tensor label: I522
    std::unique_ptr<double[]> i1data = in(1)->get_block(x1, x0, a2, x2);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x1, x0, a2, x2)]);
    sort_indices<2,0,1,3,0,1,1,1>(i1data, i1data_sorted, x1.size(), x0.size(), a2.size(), x2.size());
    dgemm_("T", "N", x5.size()*x4.size()*x3.size(), x1.size()*x0.size()*x2.size(), a2.size(),
           1.0, i0data_sorted, a2.size(), i1data_sorted, a2.size(),
           1.0, odata_sorted, x5.size()*x4.size()*x3.size());
  }
  sort_indices<3,4,5,0,1,2,1,1,1,1>(odata_sorted, odata, x5.size(), x4.size(), x3.size(), x1.size(), x0.size(), x2.size());
  out()->add_block(odata, x1, x0, x2, x5, x4, x3);
}

void Task454::Task_local::compute() {
  const Index x1 = b(0);
  const Index x0 = b(1);
  const Index a2 = b(2);
  const Index x2 = b(3);
  // tensor label: I522
  std::unique_ptr<double[]> odata(new double[out()->get_size(x1, x0, a2, x2)]);
  std::fill_n(odata.get(), out()->get_size(x1, x0, a2, x2), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x1, x0, a2, x2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x1, x0, a2, x2), 0.0);
  for (auto& c1 : *range_[0]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(c1, x2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, x2)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, c1.size(), x2.size());
    // tensor label: l2
    std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, x0, x1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, x0, x1)]);
    sort_indices<0,1,2,3,0,1,-1,1>(i1data, i1data_sorted, c1.size(), a2.size(), x0.size(), x1.size());
    dgemm_("T", "N", x2.size(), x1.size()*x0.size()*a2.size(), c1.size(),
           1.0, i0data_sorted, c1.size(), i1data_sorted, c1.size(),
           1.0, odata_sorted, x2.size());
  }
  sort_indices<3,2,1,0,1,1,1,1>(odata_sorted, odata, x2.size(), a2.size(), x0.size(), x1.size());
  out()->add_block(odata, x1, x0, a2, x2);
}

void Task455::Task_local::compute() {
  const Index x5 = b(0);
  const Index x0 = b(1);
  const Index x3 = b(2);
  const Index x4 = b(3);
  const Index x2 = b(4);
  const Index x1 = b(5);
  // tensor label: I541
  std::unique_ptr<double[]> i0data = in(0)->get_block(x5, x4, x3, x2, x1, x0);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, x4, x3, x2, x1, x0)]);
  sort_indices<0,5,2,1,3,4,0,1,1,1>(i0data, i0data_sorted, x5.size(), x4.size(), x3.size(), x2.size(), x1.size(), x0.size());
  // tensor label (calculated on-the-fly): Gamma166
  {
    if (x2 == x4) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x0, x3, x1)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x0, x3, x1), 0.0);
      for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
        for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
          for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix0+x0.size()*(ix3+x3.size()*(ix1)))] +=
                  -1.0 * i0data_sorted[ix5+x5.size()*(ix0+x0.size()*(ix3+x3.size()*(ix4+x4.size()*(ix4+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x0, x3, x1);
    }
  }
  {
    if (x3 == x4) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x0, x2, x1)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x0, x2, x1), 0.0);
      for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
        for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
          for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix0+x0.size()*(ix2+x2.size()*(ix1)))] +=
                  2.0 * i0data_sorted[ix5+x5.size()*(ix0+x0.size()*(ix4+x3.size()*(ix4+x4.size()*(ix2+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x0, x2, x1);
    }
  }
  {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x5, x0, x3, x4, x2, x1)]);
    std::fill_n(o3data.get(), out(3)->get_size(x5, x0, x3, x4, x2, x1), 0.0);
    sort_indices<0,1,2,3,4,5,1,1,-1,1>(i0data_sorted, o3data, x5.size(), x0.size(), x3.size(), x4.size(), x2.size(), x1.size());
    out(3)->add_block(o3data, x5, x0, x3, x4, x2, x1);
  }
}

void Task456::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index x2 = b(3);
  const Index x1 = b(4);
  const Index x0 = b(5);
  // tensor label: I541
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: l2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, x2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, a1, x1, x2)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x0.size(), a1.size(), x1.size(), x2.size());
    // tensor label: I542
    std::unique_ptr<double[]> i1data = in(1)->get_block(x5, a1, x4, x3);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x5, a1, x4, x3)]);
    sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, x5.size(), a1.size(), x4.size(), x3.size());
    dgemm_("T", "N", x2.size()*x1.size()*x0.size(), x5.size()*x4.size()*x3.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x2.size()*x1.size()*x0.size());
  }
  sort_indices<3,4,5,2,1,0,1,1,1,1>(odata_sorted, odata, x0.size(), x1.size(), x2.size(), x5.size(), x4.size(), x3.size());
  out()->add_block(odata, x5, x4, x3, x2, x1, x0);
}

void Task457::Task_local::compute() {
  const Index x5 = b(0);
  const Index a1 = b(1);
  const Index x4 = b(2);
  const Index x3 = b(3);
  // tensor label: I542
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, a1, x4, x3)]);
  std::fill_n(odata.get(), out()->get_size(x5, a1, x4, x3), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, a1, x4, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, a1, x4, x3), 0.0);
  for (auto& c2 : *range_[0]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, c2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, c2)]);
    sort_indices<1,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), c2.size());
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x5, a1, c2, x4);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x5, a1, c2, x4)]);
    sort_indices<2,0,1,3,0,1,1,1>(i1data, i1data_sorted, x5.size(), a1.size(), c2.size(), x4.size());
    dgemm_("T", "N", x3.size(), x5.size()*a1.size()*x4.size(), c2.size(),
           1.0, i0data_sorted, c2.size(), i1data_sorted, c2.size(),
           1.0, odata_sorted, x3.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, x3.size(), x5.size(), a1.size(), x4.size());
  out()->add_block(odata, x5, a1, x4, x3);
}

void Task458::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index x0 = b(3);
  const Index x2 = b(4);
  const Index x1 = b(5);
  // tensor label: I545
  std::unique_ptr<double[]> i0data = in(0)->get_block(x5, x4, x3, x2, x1, x0);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, x4, x3, x2, x1, x0)]);
  sort_indices<0,1,2,5,3,4,0,1,1,1>(i0data, i0data_sorted, x5.size(), x4.size(), x3.size(), x2.size(), x1.size(), x0.size());
  // tensor label (calculated on-the-fly): Gamma167
  {
    if (x2 == x4) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x1, x3, x0)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x1, x3, x0), 0.0);
      for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
        for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
          for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
            for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix1+x1.size()*(ix3+x3.size()*(ix0)))] +=
                  1.0 * i0data_sorted[ix5+x5.size()*(ix4+x4.size()*(ix3+x3.size()*(ix0+x0.size()*(ix4+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x1, x3, x0);
    }
  }
  {
    if (x3 == x4) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x0, x2, x1)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x0, x2, x1), 0.0);
      for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
        for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
          for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
            for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix0+x0.size()*(ix2+x2.size()*(ix1)))] +=
                  1.0 * i0data_sorted[ix5+x5.size()*(ix4+x4.size()*(ix4+x3.size()*(ix0+x0.size()*(ix2+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x0, x2, x1);
    }
  }
  {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x5, x4, x3, x0, x2, x1)]);
    std::fill_n(o3data.get(), out(3)->get_size(x5, x4, x3, x0, x2, x1), 0.0);
    sort_indices<0,1,2,3,4,5,1,1,1,1>(i0data_sorted, o3data, x5.size(), x4.size(), x3.size(), x0.size(), x2.size(), x1.size());
    out(3)->add_block(o3data, x5, x4, x3, x0, x2, x1);
  }
}

void Task459::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index x2 = b(3);
  const Index x1 = b(4);
  const Index x0 = b(5);
  // tensor label: I545
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: l2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, x2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, a1, x1, x2)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x0.size(), a1.size(), x1.size(), x2.size());
    // tensor label: I546
    std::unique_ptr<double[]> i1data = in(1)->get_block(a1, x5, x4, x3);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a1, x5, x4, x3)]);
    sort_indices<0,1,2,3,0,1,1,1>(i1data, i1data_sorted, a1.size(), x5.size(), x4.size(), x3.size());
    dgemm_("T", "N", x2.size()*x1.size()*x0.size(), x5.size()*x4.size()*x3.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x2.size()*x1.size()*x0.size());
  }
  sort_indices<3,4,5,2,1,0,1,1,1,1>(odata_sorted, odata, x0.size(), x1.size(), x2.size(), x5.size(), x4.size(), x3.size());
  out()->add_block(odata, x5, x4, x3, x2, x1, x0);
}

void Task460::Task_local::compute() {
  const Index a1 = b(0);
  const Index x5 = b(1);
  const Index x4 = b(2);
  const Index x3 = b(3);
  // tensor label: I546
  std::unique_ptr<double[]> odata(new double[out()->get_size(a1, x5, x4, x3)]);
  std::fill_n(odata.get(), out()->get_size(a1, x5, x4, x3), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a1, x5, x4, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a1, x5, x4, x3), 0.0);
  for (auto& c2 : *range_[0]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, c2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, c2)]);
    sort_indices<1,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), c2.size());
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(1)->get_block(c2, a1, x5, x4);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c2, a1, x5, x4)]);
    sort_indices<0,1,2,3,0,1,-1,1>(i1data, i1data_sorted, c2.size(), a1.size(), x5.size(), x4.size());
    dgemm_("T", "N", x3.size(), a1.size()*x5.size()*x4.size(), c2.size(),
           1.0, i0data_sorted, c2.size(), i1data_sorted, c2.size(),
           1.0, odata_sorted, x3.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, x3.size(), a1.size(), x5.size(), x4.size());
  out()->add_block(odata, a1, x5, x4, x3);
}

void Task461::Task_local::compute() {
  const Index x7 = b(0);
  const Index x0 = b(1);
  const Index x6 = b(2);
  const Index x5 = b(3);
  const Index x2 = b(4);
  const Index x1 = b(5);
  const Index x4 = b(6);
  const Index x3 = b(7);
  // tensor label: I549
  std::unique_ptr<double[]> i0data = in(0)->get_block(x2, x1, x0, x7, x6, x5);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x2, x1, x0, x7, x6, x5)]);
  sort_indices<3,2,4,5,0,1,0,1,1,1>(i0data, i0data_sorted, x2.size(), x1.size(), x0.size(), x7.size(), x6.size(), x5.size());
  // tensor label (calculated on-the-fly): Gamma168
  // associated with merged
  std::unique_ptr<double[]> fdata = in(1)->get_block(x4, x3);
  if (x2 == x5) {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x7, x0, x6, x1, x4, x3)]);
    std::fill_n(o3data.get(), out(3)->get_size(x7, x0, x6, x1, x4, x3), 0.0);
    for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
      for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
        for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
          for (int ix6 = 0; ix6 != x6.size(); ++ix6) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix7 = 0; ix7 != x7.size(); ++ix7) {
                for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                  o3data[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix1+x1.size()*(ix4+x4.size()*(ix3)))))] +=
                    1.0 * fdata[ix4+x4.size()*(ix3)] * i0data_sorted[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix5+x2.size()*(ix1)))))];
                }
              }
            }
          }
        }
      }
    }
    out(3)->add_block(o3data, x7, x0, x6, x1, x4, x3);
  }
  if (x4 == x5 && x2 == x3) {
    std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x7, x0, x6, x1)]);
    std::fill_n(o2data.get(), out(2)->get_size(x7, x0, x6, x1), 0.0);
    for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
      for (int ix6 = 0; ix6 != x6.size(); ++ix6) {
        for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
          for (int ix7 = 0; ix7 != x7.size(); ++ix7) {
            for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
              for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
                o2data[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix1)))] +=
                  1.0 * fdata[ix5+x4.size()*(ix3)] * i0data_sorted[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix3+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
    }
    out(2)->add_block(o2data, x7, x0, x6, x1);
  }
  if (x2 == x3) {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x7, x0, x6, x5, x4, x1)]);
    std::fill_n(o3data.get(), out(3)->get_size(x7, x0, x6, x5, x4, x1), 0.0);
    for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
      for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
        for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
          for (int ix6 = 0; ix6 != x6.size(); ++ix6) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix7 = 0; ix7 != x7.size(); ++ix7) {
                for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
                  o3data[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix4+x4.size()*(ix1)))))] +=
                    1.0 * fdata[ix4+x4.size()*(ix3)] * i0data_sorted[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix3+x2.size()*(ix1)))))];
                }
              }
            }
          }
        }
      }
    }
    out(3)->add_block(o3data, x7, x0, x6, x5, x4, x1);
  }
  if (x4 == x5) {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x7, x0, x6, x3, x2, x1)]);
    std::fill_n(o3data.get(), out(3)->get_size(x7, x0, x6, x3, x2, x1), 0.0);
    for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
      for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
        for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
          for (int ix6 = 0; ix6 != x6.size(); ++ix6) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix7 = 0; ix7 != x7.size(); ++ix7) {
                for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                  o3data[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix3+x3.size()*(ix2+x2.size()*(ix1)))))] +=
                    1.0 * fdata[ix5+x4.size()*(ix3)] * i0data_sorted[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix2+x2.size()*(ix1)))))];
                }
              }
            }
          }
        }
      }
    }
    out(3)->add_block(o3data, x7, x0, x6, x3, x2, x1);
  }
  {
    std::unique_ptr<double[]> o4data(new double[out(4)->get_size(x7, x0, x6, x5, x2, x1)]);
    std::fill_n(o4data.get(), out(4)->get_size(x7, x0, x6, x5, x2, x1), 0.0);
    for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
      for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
        for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
          for (int ix6 = 0; ix6 != x6.size(); ++ix6) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix7 = 0; ix7 != x7.size(); ++ix7) {
                o4data[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix2+x2.size()*(ix1)))))] +=
                  1.0 * i0data_sorted[ix7+x7.size()*(ix0+x0.size()*(ix6+x6.size()*(ix5+x5.size()*(ix2+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
    }
    out(4)->add_block(o4data, x7, x0, x6, x5, x2, x1);
  }
}

void Task462::Task_local::compute() {
  const Index x2 = b(0);
  const Index x1 = b(1);
  const Index x0 = b(2);
  const Index x7 = b(3);
  const Index x6 = b(4);
  const Index x5 = b(5);
  // tensor label: I549
  std::unique_ptr<double[]> odata(new double[out()->get_size(x2, x1, x0, x7, x6, x5)]);
  std::fill_n(odata.get(), out()->get_size(x2, x1, x0, x7, x6, x5), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x2, x1, x0, x7, x6, x5)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x2, x1, x0, x7, x6, x5), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x7, a1, x6, x5);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x7, a1, x6, x5)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x7.size(), a1.size(), x6.size(), x5.size());
    // tensor label: l2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, x1, x2);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, x1, x2)]);
    sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, x0.size(), a1.size(), x1.size(), x2.size());
    dgemm_("T", "N", x7.size()*x6.size()*x5.size(), x2.size()*x1.size()*x0.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x7.size()*x6.size()*x5.size());
  }
  sort_indices<5,4,3,0,1,2,1,1,1,1>(odata_sorted, odata, x7.size(), x6.size(), x5.size(), x0.size(), x1.size(), x2.size());
  out()->add_block(odata, x2, x1, x0, x7, x6, x5);
}

void Task463::Task_local::compute() {
  const Index x5 = b(0);
  const Index x0 = b(1);
  const Index x4 = b(2);
  const Index x3 = b(3);
  const Index x2 = b(4);
  const Index x1 = b(5);
  // tensor label: I552
  std::unique_ptr<double[]> i0data = in(0)->get_block(x5, x4, x3, x2, x1, x0);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, x4, x3, x2, x1, x0)]);
  sort_indices<0,5,1,2,3,4,0,1,1,1>(i0data, i0data_sorted, x5.size(), x4.size(), x3.size(), x2.size(), x1.size(), x0.size());
  // tensor label (calculated on-the-fly): Gamma169
  {
    if (x2 == x3) {
      std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x5, x0, x4, x1)]);
      std::fill_n(o2data.get(), out(2)->get_size(x5, x0, x4, x1), 0.0);
      for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
        for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
          for (int ix4 = 0; ix4 != x4.size(); ++ix4) {
            for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
              for (int ix5 = 0; ix5 != x5.size(); ++ix5) {
                o2data[ix5+x5.size()*(ix0+x0.size()*(ix4+x4.size()*(ix1)))] +=
                  1.0 * i0data_sorted[ix5+x5.size()*(ix0+x0.size()*(ix4+x4.size()*(ix3+x3.size()*(ix3+x2.size()*(ix1)))))];
              }
            }
          }
        }
      }
      out(2)->add_block(o2data, x5, x0, x4, x1);
    }
  }
  {
    std::unique_ptr<double[]> o3data(new double[out(3)->get_size(x5, x0, x4, x3, x2, x1)]);
    std::fill_n(o3data.get(), out(3)->get_size(x5, x0, x4, x3, x2, x1), 0.0);
    sort_indices<0,1,2,3,4,5,1,1,1,1>(i0data_sorted, o3data, x5.size(), x0.size(), x4.size(), x3.size(), x2.size(), x1.size());
    out(3)->add_block(o3data, x5, x0, x4, x3, x2, x1);
  }
}

void Task464::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index x2 = b(3);
  const Index x1 = b(4);
  const Index x0 = b(5);
  // tensor label: I552
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: l2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, x2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, a1, x1, x2)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x0.size(), a1.size(), x1.size(), x2.size());
    // tensor label: I553
    std::unique_ptr<double[]> i1data = in(1)->get_block(x5, x4, x3, a1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x5, x4, x3, a1)]);
    sort_indices<3,0,1,2,0,1,1,1>(i1data, i1data_sorted, x5.size(), x4.size(), x3.size(), a1.size());
    dgemm_("T", "N", x2.size()*x1.size()*x0.size(), x5.size()*x4.size()*x3.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x2.size()*x1.size()*x0.size());
  }
  sort_indices<3,4,5,2,1,0,1,1,1,1>(odata_sorted, odata, x0.size(), x1.size(), x2.size(), x5.size(), x4.size(), x3.size());
  out()->add_block(odata, x5, x4, x3, x2, x1, x0);
}

void Task465::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index a1 = b(3);
  // tensor label: I553
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, x4, x3, a1)]);
  std::fill_n(odata.get(), out()->get_size(x5, x4, x3, a1), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, x4, x3, a1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, x4, x3, a1), 0.0);
  for (auto& a2 : *range_[2]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(a2, a1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a2, a1)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a2.size(), a1.size());
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x5, a2, x4, x3);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x5, a2, x4, x3)]);
    sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, x5.size(), a2.size(), x4.size(), x3.size());
    dgemm_("T", "N", a1.size(), x5.size()*x4.size()*x3.size(), a2.size(),
           1.0, i0data_sorted, a2.size(), i1data_sorted, a2.size(),
           1.0, odata_sorted, a1.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, a1.size(), x5.size(), x4.size(), x3.size());
  out()->add_block(odata, x5, x4, x3, a1);
}

void Task466::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index a1 = b(3);
  // tensor label: I553
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, x4, x3, a1)]);
  std::fill_n(odata.get(), out()->get_size(x5, x4, x3, a1), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, x4, x3, a1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, x4, x3, a1), 0.0);
  for (auto& a2 : *range_[2]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(a2, x3);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a2, x3)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a2.size(), x3.size());
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x5, a1, x4, a2);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x5, a1, x4, a2)]);
    sort_indices<3,0,1,2,0,1,2,1>(i1data, i1data_sorted, x5.size(), a1.size(), x4.size(), a2.size());
    dgemm_("T", "N", x3.size(), x5.size()*a1.size()*x4.size(), a2.size(),
           1.0, i0data_sorted, a2.size(), i1data_sorted, a2.size(),
           1.0, odata_sorted, x3.size());
  }
  sort_indices<1,3,0,2,1,1,1,1>(odata_sorted, odata, x3.size(), x5.size(), a1.size(), x4.size());
  out()->add_block(odata, x5, x4, x3, a1);
}

void Task467::Task_local::compute() {
  const Index x5 = b(0);
  const Index x4 = b(1);
  const Index x3 = b(2);
  const Index x2 = b(3);
  const Index x1 = b(4);
  const Index x0 = b(5);
  // tensor label: I552
  std::unique_ptr<double[]> odata(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x5, x4, x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x5, x4, x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x5, a1, x4, x3);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x5, a1, x4, x3)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x5.size(), a1.size(), x4.size(), x3.size());
    // tensor label: I689
    std::unique_ptr<double[]> i1data = in(1)->get_block(x1, a1, x0, x2);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x1, a1, x0, x2)]);
    sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, x1.size(), a1.size(), x0.size(), x2.size());
    dgemm_("T", "N", x5.size()*x4.size()*x3.size(), x1.size()*x0.size()*x2.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x5.size()*x4.size()*x3.size());
  }
  sort_indices<0,1,2,5,3,4,1,1,1,1>(odata_sorted, odata, x5.size(), x4.size(), x3.size(), x1.size(), x0.size(), x2.size());
  out()->add_block(odata, x5, x4, x3, x2, x1, x0);
}

void Task468::Task_local::compute() {
  const Index x1 = b(0);
  const Index a1 = b(1);
  const Index x0 = b(2);
  const Index x2 = b(3);
  // tensor label: I689
  std::unique_ptr<double[]> odata(new double[out()->get_size(x1, a1, x0, x2)]);
  std::fill_n(odata.get(), out()->get_size(x1, a1, x0, x2), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x1, a1, x0, x2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x1, a1, x0, x2), 0.0);
  for (auto& a2 : *range_[2]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(x2, a2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x2, a2)]);
    sort_indices<1,0,0,1,1,1>(i0data, i0data_sorted, x2.size(), a2.size());
    // tensor label: l2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, x1, a2);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, x1, a2)]);
    sort_indices<3,0,1,2,0,1,2,1>(i1data, i1data_sorted, x0.size(), a1.size(), x1.size(), a2.size());
    dgemm_("T", "N", x2.size(), x1.size()*a1.size()*x0.size(), a2.size(),
           1.0, i0data_sorted, a2.size(), i1data_sorted, a2.size(),
           1.0, odata_sorted, x2.size());
  }
  sort_indices<3,2,1,0,1,1,1,1>(odata_sorted, odata, x2.size(), x0.size(), a1.size(), x1.size());
  out()->add_block(odata, x1, a1, x0, x2);
}

void Task469::Task_local::compute() {
  const Index x3 = b(0);
  const Index x0 = b(1);
  const Index x2 = b(2);
  const Index x1 = b(3);
  // tensor label: I556
  std::unique_ptr<double[]> i0data = in(0)->get_block(x3, x2, x1, x0);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, x2, x1, x0)]);
  sort_indices<0,3,1,2,0,1,1,1>(i0data, i0data_sorted, x3.size(), x2.size(), x1.size(), x0.size());
  // tensor label (calculated on-the-fly): Gamma170
  {
    std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x3, x0, x2, x1)]);
    std::fill_n(o2data.get(), out(2)->get_size(x3, x0, x2, x1), 0.0);
    sort_indices<0,1,2,3,1,1,1,1>(i0data_sorted, o2data, x3.size(), x0.size(), x2.size(), x1.size());
    out(2)->add_block(o2data, x3, x0, x2, x1);
  }
}

void Task470::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I556
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: l2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, x2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, a1, x1, x2)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x0.size(), a1.size(), x1.size(), x2.size());
    // tensor label: I557
    std::unique_ptr<double[]> i1data = in(1)->get_block(x3, a1);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, a1)]);
    sort_indices<1,0,0,1,1,1>(i1data, i1data_sorted, x3.size(), a1.size());
    dgemm_("T", "N", x2.size()*x1.size()*x0.size(), x3.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x2.size()*x1.size()*x0.size());
  }
  sort_indices<3,2,1,0,1,1,1,1>(odata_sorted, odata, x0.size(), x1.size(), x2.size(), x3.size());
  out()->add_block(odata, x3, x2, x1, x0);
}

void Task471::Task_local::compute() {
  const Index x3 = b(0);
  const Index a1 = b(1);
  // tensor label: I557
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, a1)]);
  std::fill_n(odata.get(), out()->get_size(x3, a1), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, a1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, a1), 0.0);
  for (auto& a3 : *range_[2]) {
    for (auto& c2 : *range_[0]) {
      // tensor label: f1
      std::unique_ptr<double[]> i0data = in(0)->get_block(a3, c2);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a3, c2)]);
      sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a3.size(), c2.size());
      // tensor label: I558
      std::unique_ptr<double[]> i1data = in(1)->get_block(x3, a3, c2, a1);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, a3, c2, a1)]);
      sort_indices<1,2,0,3,0,1,1,1>(i1data, i1data_sorted, x3.size(), a3.size(), c2.size(), a1.size());
      dgemm_("T", "N", 1, x3.size()*a1.size(), a3.size()*c2.size(),
             1.0, i0data_sorted, a3.size()*c2.size(), i1data_sorted, a3.size()*c2.size(),
             1.0, odata_sorted, 1);
    }
  }
  sort_indices<0,1,1,1,1,1>(odata_sorted, odata, x3.size(), a1.size());
  out()->add_block(odata, x3, a1);
}

void Task472::Task_local::compute() {
  const Index x3 = b(0);
  const Index a3 = b(1);
  const Index c2 = b(2);
  const Index a1 = b(3);
  // tensor label: I558
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, a3, c2, a1)]);
  std::fill_n(odata.get(), out()->get_size(x3, a3, c2, a1), 0.0);
  {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a3, c2, a1);
    sort_indices<0,1,2,3,1,1,-1,1>(i0data, odata, x3.size(), a3.size(), c2.size(), a1.size());
  }
  {
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(0)->get_block(x3, a1, c2, a3);
    sort_indices<0,3,2,1,1,1,2,1>(i1data, odata, x3.size(), a1.size(), c2.size(), a3.size());
  }
  out()->add_block(odata, x3, a3, c2, a1);
}

void Task473::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I556
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  for (auto& a3 : *range_[2]) {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a3, x2, x1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a3, x2, x1)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x3.size(), a3.size(), x2.size(), x1.size());
    // tensor label: I639
    std::unique_ptr<double[]> i1data = in(1)->get_block(a3, x0);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a3, x0)]);
    sort_indices<0,1,0,1,1,1>(i1data, i1data_sorted, a3.size(), x0.size());
    dgemm_("T", "N", x3.size()*x2.size()*x1.size(), x0.size(), a3.size(),
           1.0, i0data_sorted, a3.size(), i1data_sorted, a3.size(),
           1.0, odata_sorted, x3.size()*x2.size()*x1.size());
  }
  sort_indices<0,1,2,3,1,1,1,1>(odata_sorted, odata, x3.size(), x2.size(), x1.size(), x0.size());
  out()->add_block(odata, x3, x2, x1, x0);
}

void Task474::Task_local::compute() {
  const Index a3 = b(0);
  const Index x0 = b(1);
  // tensor label: I639
  std::unique_ptr<double[]> odata(new double[out()->get_size(a3, x0)]);
  std::fill_n(odata.get(), out()->get_size(a3, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a3, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a3, x0), 0.0);
  for (auto& c2 : *range_[0]) {
    for (auto& a1 : *range_[2]) {
      // tensor label: f1
      std::unique_ptr<double[]> i0data = in(0)->get_block(c2, a1);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c2, a1)]);
      sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, c2.size(), a1.size());
      // tensor label: l2
      std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, c2, a3);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, c2, a3)]);
      sort_indices<2,1,0,3,0,1,-1,1>(i1data, i1data_sorted, x0.size(), a1.size(), c2.size(), a3.size());
      dgemm_("T", "N", 1, a3.size()*x0.size(), c2.size()*a1.size(),
             1.0, i0data_sorted, c2.size()*a1.size(), i1data_sorted, c2.size()*a1.size(),
             1.0, odata_sorted, 1);
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x0.size(), a3.size());
  out()->add_block(odata, a3, x0);
}

void Task475::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I556
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    // tensor label: t2
    std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a1, x2, x1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a1, x2, x1)]);
    sort_indices<1,0,2,3,0,1,1,1>(i0data, i0data_sorted, x3.size(), a1.size(), x2.size(), x1.size());
    // tensor label: I643
    std::unique_ptr<double[]> i1data = in(1)->get_block(a1, x0);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a1, x0)]);
    sort_indices<0,1,0,1,1,1>(i1data, i1data_sorted, a1.size(), x0.size());
    dgemm_("T", "N", x3.size()*x2.size()*x1.size(), x0.size(), a1.size(),
           1.0, i0data_sorted, a1.size(), i1data_sorted, a1.size(),
           1.0, odata_sorted, x3.size()*x2.size()*x1.size());
  }
  sort_indices<0,1,2,3,1,1,1,1>(odata_sorted, odata, x3.size(), x2.size(), x1.size(), x0.size());
  out()->add_block(odata, x3, x2, x1, x0);
}

void Task476::Task_local::compute() {
  const Index a1 = b(0);
  const Index x0 = b(1);
  // tensor label: I643
  std::unique_ptr<double[]> odata(new double[out()->get_size(a1, x0)]);
  std::fill_n(odata.get(), out()->get_size(a1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a1, x0), 0.0);
  for (auto& c2 : *range_[0]) {
    for (auto& a3 : *range_[2]) {
      // tensor label: f1
      std::unique_ptr<double[]> i0data = in(0)->get_block(c2, a3);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c2, a3)]);
      sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, c2.size(), a3.size());
      // tensor label: l2
      std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, c2, a3);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, c2, a3)]);
      sort_indices<2,3,0,1,0,1,2,1>(i1data, i1data_sorted, x0.size(), a1.size(), c2.size(), a3.size());
      dgemm_("T", "N", 1, a1.size()*x0.size(), a3.size()*c2.size(),
             1.0, i0data_sorted, a3.size()*c2.size(), i1data_sorted, a3.size()*c2.size(),
             1.0, odata_sorted, 1);
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x0.size(), a1.size());
  out()->add_block(odata, a1, x0);
}

void Task477::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I556
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  for (auto& a1 : *range_[2]) {
    for (auto& a3 : *range_[2]) {
      // tensor label: t2
      std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a1, x2, a3);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a1, x2, a3)]);
      sort_indices<1,3,0,2,0,1,1,1>(i0data, i0data_sorted, x3.size(), a1.size(), x2.size(), a3.size());
      // tensor label: I685
      std::unique_ptr<double[]> i1data = in(1)->get_block(a3, a1, x0, x1);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a3, a1, x0, x1)]);
      sort_indices<1,0,2,3,0,1,1,1>(i1data, i1data_sorted, a3.size(), a1.size(), x0.size(), x1.size());
      dgemm_("T", "N", x3.size()*x2.size(), x0.size()*x1.size(), a3.size()*a1.size(),
             1.0, i0data_sorted, a3.size()*a1.size(), i1data_sorted, a3.size()*a1.size(),
             1.0, odata_sorted, x3.size()*x2.size());
    }
  }
  sort_indices<0,1,3,2,1,1,1,1>(odata_sorted, odata, x3.size(), x2.size(), x0.size(), x1.size());
  out()->add_block(odata, x3, x2, x1, x0);
}

void Task478::Task_local::compute() {
  const Index a3 = b(0);
  const Index a1 = b(1);
  const Index x0 = b(2);
  const Index x1 = b(3);
  // tensor label: I685
  std::unique_ptr<double[]> odata(new double[out()->get_size(a3, a1, x0, x1)]);
  std::fill_n(odata.get(), out()->get_size(a3, a1, x0, x1), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a3, a1, x0, x1)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a3, a1, x0, x1), 0.0);
  for (auto& c2 : *range_[0]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(c2, x1);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c2, x1)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, c2.size(), x1.size());
    // tensor label: l2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, c2, a3);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, c2, a3)]);
    sort_indices<2,0,1,3,0,1,-2,1>(i1data, i1data_sorted, x0.size(), a1.size(), c2.size(), a3.size());
    dgemm_("T", "N", x1.size(), a3.size()*a1.size()*x0.size(), c2.size(),
           1.0, i0data_sorted, c2.size(), i1data_sorted, c2.size(),
           1.0, odata_sorted, x1.size());
  }
  sort_indices<3,2,1,0,1,1,1,1>(odata_sorted, odata, x1.size(), x0.size(), a1.size(), a3.size());
  out()->add_block(odata, a3, a1, x0, x1);
}

void Task479::Task_local::compute() {
  const Index x3 = b(0);
  const Index x2 = b(1);
  const Index x1 = b(2);
  const Index x0 = b(3);
  // tensor label: I556
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, x2, x1, x0)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, x2, x1, x0), 0.0);
  for (auto& a2 : *range_[2]) {
    for (auto& a1 : *range_[2]) {
      // tensor label: l2
      std::unique_ptr<double[]> i0data = in(0)->get_block(x0, a1, x1, a2);
      std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, a1, x1, a2)]);
      sort_indices<3,1,0,2,0,1,1,1>(i0data, i0data_sorted, x0.size(), a1.size(), x1.size(), a2.size());
      // tensor label: I693
      std::unique_ptr<double[]> i1data = in(1)->get_block(x3, a1, a2, x2);
      std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, a1, a2, x2)]);
      sort_indices<2,1,0,3,0,1,1,1>(i1data, i1data_sorted, x3.size(), a1.size(), a2.size(), x2.size());
      dgemm_("T", "N", x1.size()*x0.size(), x3.size()*x2.size(), a1.size()*a2.size(),
             1.0, i0data_sorted, a1.size()*a2.size(), i1data_sorted, a1.size()*a2.size(),
             1.0, odata_sorted, x1.size()*x0.size());
    }
  }
  sort_indices<2,3,1,0,1,1,1,1>(odata_sorted, odata, x0.size(), x1.size(), x3.size(), x2.size());
  out()->add_block(odata, x3, x2, x1, x0);
}

void Task480::Task_local::compute() {
  const Index x3 = b(0);
  const Index a1 = b(1);
  const Index a2 = b(2);
  const Index x2 = b(3);
  // tensor label: I693
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, a1, a2, x2)]);
  std::fill_n(odata.get(), out()->get_size(x3, a1, a2, x2), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, a1, a2, x2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, a1, a2, x2), 0.0);
  for (auto& c3 : *range_[0]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(x2, c3);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x2, c3)]);
    sort_indices<1,0,0,1,1,1>(i0data, i0data_sorted, x2.size(), c3.size());
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x3, a1, c3, a2);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, a1, c3, a2)]);
    sort_indices<2,0,1,3,0,1,-2,1>(i1data, i1data_sorted, x3.size(), a1.size(), c3.size(), a2.size());
    dgemm_("T", "N", x2.size(), x3.size()*a1.size()*a2.size(), c3.size(),
           1.0, i0data_sorted, c3.size(), i1data_sorted, c3.size(),
           1.0, odata_sorted, x2.size());
  }
  sort_indices<1,2,3,0,1,1,1,1>(odata_sorted, odata, x2.size(), x3.size(), a1.size(), a2.size());
  out()->add_block(odata, x3, a1, a2, x2);
}

void Task481::Task_local::compute() {
  const Index x3 = b(0);
  const Index a1 = b(1);
  const Index a2 = b(2);
  const Index x2 = b(3);
  // tensor label: I693
  std::unique_ptr<double[]> odata(new double[out()->get_size(x3, a1, a2, x2)]);
  std::fill_n(odata.get(), out()->get_size(x3, a1, a2, x2), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x3, a1, a2, x2)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x3, a1, a2, x2), 0.0);
  for (auto& a3 : *range_[2]) {
    // tensor label: f1
    std::unique_ptr<double[]> i0data = in(0)->get_block(a3, a2);
    std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a3, a2)]);
    sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a3.size(), a2.size());
    // tensor label: t2
    std::unique_ptr<double[]> i1data = in(1)->get_block(x3, a1, x2, a3);
    std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x3, a1, x2, a3)]);
    sort_indices<3,0,1,2,0,1,4,1>(i1data, i1data_sorted, x3.size(), a1.size(), x2.size(), a3.size());
    dgemm_("T", "N", a2.size(), x3.size()*a1.size()*x2.size(), a3.size(),
           1.0, i0data_sorted, a3.size(), i1data_sorted, a3.size(),
           1.0, odata_sorted, a2.size());
  }
  sort_indices<1,2,0,3,1,1,1,1>(odata_sorted, odata, a2.size(), x3.size(), a1.size(), x2.size());
  out()->add_block(odata, x3, a1, a2, x2);
}

void Task482::Task_local::compute() {
  const Index x1 = b(0);
  const Index x0 = b(1);
  // tensor label: I592
  std::unique_ptr<double[]> i0data = in(0)->get_block();
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size()]);
  sort_indices<0,1,1,1>(i0data, i0data_sorted);
  // tensor label (calculated on-the-fly): Gamma179
  // associated with merged
  std::unique_ptr<double[]> fdata = in(1)->get_block(x1, x0);
  {
    std::unique_ptr<double[]> o1data(new double[out(1)->get_size(x1, x0)]);
    std::fill_n(o1data.get(), out(1)->get_size(x1, x0), 0.0);
    for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
      for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
        o1data[ix1+x1.size()*(ix0)] +=
          1.0 * fdata[ix1+x1.size()*(ix0)] * i0data_sorted[0];
      }
    }
    out(1)->add_block(o1data, x1, x0);
  }
}

void Task483::Task_local::compute() {
  // tensor label: I592
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size()]);
  std::fill_n(odata_sorted.get(), out()->get_size(), 0.0);
  const Index c1 = b(0);
  const Index a4 = b(1);
  const Index c3 = b(2);
  const Index a2 = b(3);
  // tensor label: t2
  std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a4, c3, a2);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a4, c3, a2)]);
  sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, c1.size(), a4.size(), c3.size(), a2.size());
  // tensor label: l2
  std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, c3, a4);
  std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, c3, a4)]);
  sort_indices<0,3,2,1,0,1,-4,1>(i1data, i1data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
  odata_sorted[0] += ddot_(a4.size()*c3.size()*a2.size()*c1.size(), i0data_sorted, 1, i1data_sorted, 1);
  sort_indices<1,1,1,1>(odata_sorted, odata);
  out()->add_block(odata);
}

void Task484::Task_local::compute() {
  // tensor label: I592
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size()]);
  std::fill_n(odata_sorted.get(), out()->get_size(), 0.0);
  const Index c1 = b(0);
  const Index a2 = b(1);
  const Index c3 = b(2);
  const Index a4 = b(3);
  // tensor label: t2
  std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, a4);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, a4)]);
  sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
  // tensor label: l2
  std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, c3, a4);
  std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, c3, a4)]);
  sort_indices<0,1,2,3,0,1,8,1>(i1data, i1data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
  odata_sorted[0] += ddot_(a4.size()*c3.size()*a2.size()*c1.size(), i0data_sorted, 1, i1data_sorted, 1);
  sort_indices<1,1,1,1>(odata_sorted, odata);
  out()->add_block(odata);
}

void Task485::Task_local::compute() {
  // tensor label: I324
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  // tensor label: I598
  std::unique_ptr<double[]> i0data = in(0)->get_block();
  odata[0] += 8.0 * i0data[0];
  out()->add_block(odata);
}

void Task486::Task_local::compute() {
  // tensor label: I598
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size()]);
  std::fill_n(odata_sorted.get(), out()->get_size(), 0.0);
  const Index c3 = b(0);
  const Index c5 = b(1);
  // tensor label: f1
  std::unique_ptr<double[]> i0data = in(0)->get_block(c3, c5);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c3, c5)]);
  sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, c3.size(), c5.size());
  // tensor label: I599
  std::unique_ptr<double[]> i1data = in(1)->get_block(c3, c5);
  std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, c5)]);
  sort_indices<0,1,0,1,1,1>(i1data, i1data_sorted, c3.size(), c5.size());
  odata_sorted[0] += ddot_(c3.size()*c5.size(), i0data_sorted, 1, i1data_sorted, 1);
  sort_indices<1,1,1,1>(odata_sorted, odata);
  out()->add_block(odata);
}

void Task487::Task_local::compute() {
  const Index c3 = b(0);
  const Index c5 = b(1);
  // tensor label: I599
  std::unique_ptr<double[]> odata(new double[out()->get_size(c3, c5)]);
  std::fill_n(odata.get(), out()->get_size(c3, c5), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, c5)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, c5), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& a4 : *range_[2]) {
      for (auto& a2 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a4, c5, a2);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a4, c5, a2)]);
        sort_indices<0,1,3,2,0,1,1,1>(i0data, i0data_sorted, c1.size(), a4.size(), c5.size(), a2.size());
        // tensor label: l2
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, c3, a4);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, c3, a4)]);
        sort_indices<0,3,1,2,0,1,8,1>(i1data, i1data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
        dgemm_("T", "N", c5.size(), c3.size(), a4.size()*a2.size()*c1.size(),
               1.0, i0data_sorted, a4.size()*a2.size()*c1.size(), i1data_sorted, a4.size()*a2.size()*c1.size(),
               1.0, odata_sorted, c5.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c5.size(), c3.size());
  out()->add_block(odata, c3, c5);
}

void Task488::Task_local::compute() {
  // tensor label: I324
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  // tensor label: I602
  std::unique_ptr<double[]> i0data = in(0)->get_block();
  odata[0] += -16.0 * i0data[0];
  out()->add_block(odata);
}

void Task489::Task_local::compute() {
  // tensor label: I602
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size()]);
  std::fill_n(odata_sorted.get(), out()->get_size(), 0.0);
  const Index c3 = b(0);
  const Index c5 = b(1);
  // tensor label: f1
  std::unique_ptr<double[]> i0data = in(0)->get_block(c3, c5);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c3, c5)]);
  sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, c3.size(), c5.size());
  // tensor label: I603
  std::unique_ptr<double[]> i1data = in(1)->get_block(c3, c5);
  std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c3, c5)]);
  sort_indices<0,1,0,1,1,1>(i1data, i1data_sorted, c3.size(), c5.size());
  odata_sorted[0] += ddot_(c3.size()*c5.size(), i0data_sorted, 1, i1data_sorted, 1);
  sort_indices<1,1,1,1>(odata_sorted, odata);
  out()->add_block(odata);
}

void Task490::Task_local::compute() {
  const Index c3 = b(0);
  const Index c5 = b(1);
  // tensor label: I603
  std::unique_ptr<double[]> odata(new double[out()->get_size(c3, c5)]);
  std::fill_n(odata.get(), out()->get_size(c3, c5), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(c3, c5)]);
  std::fill_n(odata_sorted.get(), out()->get_size(c3, c5), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& a2 : *range_[2]) {
      for (auto& a4 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c5, a4);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c5, a4)]);
        sort_indices<0,1,3,2,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c5.size(), a4.size());
        // tensor label: l2
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, c3, a4);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, c3, a4)]);
        sort_indices<0,1,3,2,0,1,-16,1>(i1data, i1data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
        dgemm_("T", "N", c5.size(), c3.size(), a4.size()*a2.size()*c1.size(),
               1.0, i0data_sorted, a4.size()*a2.size()*c1.size(), i1data_sorted, a4.size()*a2.size()*c1.size(),
               1.0, odata_sorted, c5.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, c5.size(), c3.size());
  out()->add_block(odata, c3, c5);
}

void Task491::Task_local::compute() {
  // tensor label: I324
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  // tensor label: I606
  std::unique_ptr<double[]> i0data = in(0)->get_block();
  odata[0] += -8.0 * i0data[0];
  out()->add_block(odata);
}

void Task492::Task_local::compute() {
  // tensor label: I606
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size()]);
  std::fill_n(odata_sorted.get(), out()->get_size(), 0.0);
  const Index a5 = b(0);
  const Index a4 = b(1);
  // tensor label: f1
  std::unique_ptr<double[]> i0data = in(0)->get_block(a5, a4);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a5, a4)]);
  sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a5.size(), a4.size());
  // tensor label: I607
  std::unique_ptr<double[]> i1data = in(1)->get_block(a4, a5);
  std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a4, a5)]);
  sort_indices<1,0,0,1,1,1>(i1data, i1data_sorted, a4.size(), a5.size());
  odata_sorted[0] += ddot_(a4.size()*a5.size(), i0data_sorted, 1, i1data_sorted, 1);
  sort_indices<1,1,1,1>(odata_sorted, odata);
  out()->add_block(odata);
}

void Task493::Task_local::compute() {
  const Index a4 = b(0);
  const Index a5 = b(1);
  // tensor label: I607
  std::unique_ptr<double[]> odata(new double[out()->get_size(a4, a5)]);
  std::fill_n(odata.get(), out()->get_size(a4, a5), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a4, a5)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a4, a5), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& c3 : *range_[0]) {
      for (auto& a2 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a5, c3, a2);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a5, c3, a2)]);
        sort_indices<0,2,3,1,0,1,1,1>(i0data, i0data_sorted, c1.size(), a5.size(), c3.size(), a2.size());
        // tensor label: l2
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, c3, a4);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, c3, a4)]);
        sort_indices<0,2,1,3,0,1,-8,1>(i1data, i1data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
        dgemm_("T", "N", a5.size(), a4.size(), c3.size()*a2.size()*c1.size(),
               1.0, i0data_sorted, c3.size()*a2.size()*c1.size(), i1data_sorted, c3.size()*a2.size()*c1.size(),
               1.0, odata_sorted, a5.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a5.size(), a4.size());
  out()->add_block(odata, a4, a5);
}

void Task494::Task_local::compute() {
  // tensor label: I324
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  // tensor label: I610
  std::unique_ptr<double[]> i0data = in(0)->get_block();
  odata[0] += 16.0 * i0data[0];
  out()->add_block(odata);
}

void Task495::Task_local::compute() {
  // tensor label: I610
  std::unique_ptr<double[]> odata(new double[out()->get_size()]);
  std::fill_n(odata.get(), out()->get_size(), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size()]);
  std::fill_n(odata_sorted.get(), out()->get_size(), 0.0);
  const Index a5 = b(0);
  const Index a4 = b(1);
  // tensor label: f1
  std::unique_ptr<double[]> i0data = in(0)->get_block(a5, a4);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(a5, a4)]);
  sort_indices<0,1,0,1,1,1>(i0data, i0data_sorted, a5.size(), a4.size());
  // tensor label: I611
  std::unique_ptr<double[]> i1data = in(1)->get_block(a4, a5);
  std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(a4, a5)]);
  sort_indices<1,0,0,1,1,1>(i1data, i1data_sorted, a4.size(), a5.size());
  odata_sorted[0] += ddot_(a4.size()*a5.size(), i0data_sorted, 1, i1data_sorted, 1);
  sort_indices<1,1,1,1>(odata_sorted, odata);
  out()->add_block(odata);
}

void Task496::Task_local::compute() {
  const Index a4 = b(0);
  const Index a5 = b(1);
  // tensor label: I611
  std::unique_ptr<double[]> odata(new double[out()->get_size(a4, a5)]);
  std::fill_n(odata.get(), out()->get_size(a4, a5), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(a4, a5)]);
  std::fill_n(odata_sorted.get(), out()->get_size(a4, a5), 0.0);
  for (auto& c1 : *range_[0]) {
    for (auto& a2 : *range_[2]) {
      for (auto& c3 : *range_[0]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(c1, a2, c3, a5);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(c1, a2, c3, a5)]);
        sort_indices<0,1,2,3,0,1,1,1>(i0data, i0data_sorted, c1.size(), a2.size(), c3.size(), a5.size());
        // tensor label: l2
        std::unique_ptr<double[]> i1data = in(1)->get_block(c1, a2, c3, a4);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(c1, a2, c3, a4)]);
        sort_indices<0,1,2,3,0,1,16,1>(i1data, i1data_sorted, c1.size(), a2.size(), c3.size(), a4.size());
        dgemm_("T", "N", a5.size(), a4.size(), c3.size()*a2.size()*c1.size(),
               1.0, i0data_sorted, c3.size()*a2.size()*c1.size(), i1data_sorted, c3.size()*a2.size()*c1.size(),
               1.0, odata_sorted, a5.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, a5.size(), a4.size());
  out()->add_block(odata, a4, a5);
}

void Task497::Task_local::compute() {
  const Index x3 = b(0);
  const Index x0 = b(1);
  const Index x2 = b(2);
  const Index x1 = b(3);
  // tensor label: I654
  std::unique_ptr<double[]> i0data = in(0)->get_block(x0, x3);
  std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x0, x3)]);
  sort_indices<1,0,0,1,1,1>(i0data, i0data_sorted, x0.size(), x3.size());
  // tensor label (calculated on-the-fly): Gamma191
  // associated with merged
  std::unique_ptr<double[]> fdata = in(1)->get_block(x2, x1);
  {
    std::unique_ptr<double[]> o2data(new double[out(2)->get_size(x3, x0, x2, x1)]);
    std::fill_n(o2data.get(), out(2)->get_size(x3, x0, x2, x1), 0.0);
    for (int ix1 = 0; ix1 != x1.size(); ++ix1) {
      for (int ix2 = 0; ix2 != x2.size(); ++ix2) {
        for (int ix0 = 0; ix0 != x0.size(); ++ix0) {
          for (int ix3 = 0; ix3 != x3.size(); ++ix3) {
            o2data[ix3+x3.size()*(ix0+x0.size()*(ix2+x2.size()*(ix1)))] +=
              1.0 * fdata[ix2+x2.size()*(ix1)] * i0data_sorted[ix3+x3.size()*(ix0)];
          }
        }
      }
    }
    out(2)->add_block(o2data, x3, x0, x2, x1);
  }
}

void Task498::Task_local::compute() {
  const Index x0 = b(0);
  const Index x3 = b(1);
  // tensor label: I654
  std::unique_ptr<double[]> odata(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata.get(), out()->get_size(x0, x3), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x0, x3), 0.0);
  for (auto& a3 : *range_[2]) {
    for (auto& c2 : *range_[0]) {
      for (auto& a1 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a3, c2, a1);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a3, c2, a1)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), a3.size(), c2.size(), a1.size());
        // tensor label: l2
        std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, c2, a3);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, c2, a3)]);
        sort_indices<3,2,1,0,0,1,-1,1>(i1data, i1data_sorted, x0.size(), a1.size(), c2.size(), a3.size());
        dgemm_("T", "N", x3.size(), x0.size(), a3.size()*c2.size()*a1.size(),
               1.0, i0data_sorted, a3.size()*c2.size()*a1.size(), i1data_sorted, a3.size()*c2.size()*a1.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), x0.size());
  out()->add_block(odata, x0, x3);
}

void Task499::Task_local::compute() {
  const Index x0 = b(0);
  const Index x3 = b(1);
  // tensor label: I654
  std::unique_ptr<double[]> odata(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata.get(), out()->get_size(x0, x3), 0.0);
  std::unique_ptr<double[]> odata_sorted(new double[out()->get_size(x0, x3)]);
  std::fill_n(odata_sorted.get(), out()->get_size(x0, x3), 0.0);
  for (auto& a1 : *range_[2]) {
    for (auto& c2 : *range_[0]) {
      for (auto& a3 : *range_[2]) {
        // tensor label: t2
        std::unique_ptr<double[]> i0data = in(0)->get_block(x3, a1, c2, a3);
        std::unique_ptr<double[]> i0data_sorted(new double[in(0)->get_size(x3, a1, c2, a3)]);
        sort_indices<1,2,3,0,0,1,1,1>(i0data, i0data_sorted, x3.size(), a1.size(), c2.size(), a3.size());
        // tensor label: l2
        std::unique_ptr<double[]> i1data = in(1)->get_block(x0, a1, c2, a3);
        std::unique_ptr<double[]> i1data_sorted(new double[in(1)->get_size(x0, a1, c2, a3)]);
        sort_indices<1,2,3,0,0,1,2,1>(i1data, i1data_sorted, x0.size(), a1.size(), c2.size(), a3.size());
        dgemm_("T", "N", x3.size(), x0.size(), a3.size()*c2.size()*a1.size(),
               1.0, i0data_sorted, a3.size()*c2.size()*a1.size(), i1data_sorted, a3.size()*c2.size()*a1.size(),
               1.0, odata_sorted, x3.size());
      }
    }
  }
  sort_indices<1,0,1,1,1,1>(odata_sorted, odata, x3.size(), x0.size());
  out()->add_block(odata, x0, x3);
}

#endif
