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
  vector<vector<double>> dist;
  vector<vector<double>> prev;
  int sz;

 public:
  Table(int sz);
  ostream & Print(ostream &os) const;
  void AddRow();
  void RemoveRow(int index);
  void UpdateRow(int index, vector<double> dist, vector<double> prev);
  void GetRow(int index);
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
