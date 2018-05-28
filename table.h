#ifndef _table
#define _table


#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <map>

#include "link.h"

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
class Table {
  // Students should write this class
 public:
  unsigned index;
  map<unsigned, unsigned> next_hop;
  map<unsigned, map<unsigned, Link> > t;
  map<unsigned, map<unsigned, unsigned> > id;

  Table();
  Table(unsigned ind);
  Table(const Table &rhs);
  ostream & Print(ostream &os) const;

  bool ComputeDijkstra();
  bool UpdateLink(const Link l);
  bool UpdateMessageLink(unsigned id, unsigned sender, const Link l);

  unsigned GetLinkID(unsigned src, unsigned dest);
  unsigned GetNextHop(unsigned ind);

};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>

class Table {
  // Table: Holds the DV algorithm and data structures
  // We implement DV algorithm here
  // Must be able to calculate next_hop as well
 private:
  unsigned size;
  unsigned index;
  std::vector<unsigned> next_hop;
  std::vector<double> link_cost;
  std::vector<std::vector<double> > matrix;

 public:
  Table();
  Table(unsigned num, unsigned sz);
  Table(const Table &rhs);
  ostream & Print(ostream &os) const;

  bool ComputeMatrix();
  bool UpdateMatrix(unsigned ind, vector<double> vec);
  bool UpdateLink(unsigned n, double d);
  unsigned GetNextHop(unsigned ind) const;
  std::vector<double> GetVector() const;

};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
