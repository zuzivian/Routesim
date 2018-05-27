#ifndef _table
#define _table


#include <iostream>

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
  ostream & Print(ostream &os) const;
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
  vector<unsigned> next_hop;
  vector<double> link_cost;
  vector<vector<double>> matrix;

 public:
  Table();
  Table(unsigned num, unsigned sz);
  Table(const Table &rhs);
  ostream & Print(ostream &os) const;

  bool ComputeMatrix();
  bool UpdateMatrix(unsigned index, vector<double> vec);
  bool UpdateLink(unsigned n, double d);
  unsigned GetNextHop(unsigned n);
  vector<double> GetVector(int index);

};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
