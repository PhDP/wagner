#include <iostream>
#include <sstream>
#include <ostream>
#include <string>
#include "wagner/common.hh"
#include "wagner/tbranch.hh"

namespace wagner {

tbranch::tbranch(tbranch *p, tbranch *l, tbranch *r)
    : m_parent(p), m_left(l), m_right(r) {
  m_end_date = 0;
}

tbranch::~tbranch() {
  delete m_left;
  delete m_right;
}

tbranch *tbranch::parent() const {
  return m_parent;
}

tbranch *tbranch::left() const {
  return m_left;
}

tbranch *tbranch::right() const {
  return m_right;
}

void tbranch::set_parent(tbranch *t) {
  m_parent = t;
}

void tbranch::set_left(tbranch *t) {
  m_left = t;
}

void tbranch::set_right(tbranch *t) {
  m_right = t;
}

bool tbranch::internal() const {
  return m_parent != nullptr && m_left != nullptr;
}

bool tbranch::root() const {
  return m_parent == nullptr;
}

bool tbranch::leaf() const {
  return m_left == nullptr; // Assumes the node is strictly binary.
}

size_t tbranch::end_date() const {
  return m_end_date;
}

void tbranch::set_end_date(size_t date) {
  m_end_date = date;
}

size_t tbranch::max_end_date() const {
  if (leaf()) {
    return m_end_date;
  } else {
    const int l_max = m_left->max_end_date();
    const int r_max = m_right->max_end_date();
    return max2(l_max, r_max);
  }
}

size_t tbranch::parent_distance() const {
  return (m_parent == nullptr) ? 0 : m_end_date - m_parent->end_date();
}

size_t tbranch::total_distance() const {
  return (m_parent == nullptr) ? 0 : parent_distance() + m_parent->total_distance();
}

size_t tbranch::nodes() const {
  return (m_parent == nullptr) ? 0 : 1 + m_parent->nodes();
}

size_t tbranch::edges() const {
  return (m_left == nullptr) ? 0 : 2 + m_left->edges() + m_right->edges();
}

size_t tbranch::leaves() const {
  return 1 + edges() / 2;
}

bool tbranch::strictly_binary() const {
  if (m_left == nullptr && m_right == nullptr) {
    return true;
  } else if ((m_left == nullptr && m_right != nullptr) ||
             (m_left != nullptr && m_right == nullptr)) {
    return false;
  } else {
    return m_left->strictly_binary() && m_right->strictly_binary();
  }
}

size_t tbranch::level() const {
  return leaf() ? 0 : 1 + max2(m_left->level(), m_right->level());
}

std::string tbranch::newick() const {
  std::ostringstream o;
  o << "(" << m_left->newick() << "," << m_right->newick()
    << "):" << parent_distance();
  if (root()) {
    o << ";";
  }
  return o.str();
}

}
