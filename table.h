#ifndef _table
#define _table


#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <map>

class Link;

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
  map<int, double > next_hop;
  // need a structure here to capture network topology-> vector/map of Links?

  Table();
  Table(unsigned num);
  Table(const Table &rhs);
  ostream & Print(ostream &os) const;

  bool ComputeDijkstra();
  bool UpdateLink(const Link l);
  bool UpdateMessageLink(unsigned id, unsigned sender, const Link l);

  Link GetLink(unsigned src, unsigned dest);
  unsigned GetLinkID(unsigned src, unsigned dest);
  unsigned GetNextHop(unsigned ind) const;

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
