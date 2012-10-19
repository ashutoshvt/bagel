//
// BAGEL - Parallel electron correlation program.
// Filename: queue.h
// Copyright (C) 2012 Toru Shiozaki
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


// Impliments a Task queue; in the future this should be replaced by
// some standard interface (boost::graph?)

#ifndef __SRC_SMITH_QUEUE_H
#define __SRC_SMITH_QUEUE_H

#include <src/smith/task.h>
#include <cassert>
#include <list>
#include <memory>

namespace bagel {
namespace SMITH {

template <typename T>
class Queue {
  protected:
    std::list<std::shared_ptr<Task<T> > > tasklist_;

  public:
    Queue() {};
    Queue(std::list<std::shared_ptr<Task<T> > > d) : tasklist_(d) { std::random_shuffle(tasklist_.begin(), tasklist_.end()); };
    ~Queue() {};

    // TODO parallel version to be implemented (need to WAIT!)
    std::shared_ptr<Task<T> > next_compute() {
      auto i = tasklist_.begin();
      for ( ; i != tasklist_.end(); ++i)
        if ((*i)->ready()) break;

      assert(i != tasklist_.end());
      std::shared_ptr<Task<T> > out = *i;
      // execute
      out->compute();
      // delete dependency (to remove intermediate storages)
      for (auto& j : tasklist_) j->delete_dep(out);
      // delete this task from the queue
      tasklist_.erase(i);
      return out;
    };

    void add_task(std::shared_ptr<Task<T> > a) { tasklist_.push_back(a); };

    bool done() const { return tasklist_.empty(); };

    void initialize() {
      for (auto& i : tasklist_) i->initialize();
    };
};

}
}

#endif
