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
  vector<vector<double>> matrix;
  int sz;

 public:
  Table();
  ostream & Print(ostream &os) const;
  void AddRow();
  void RemoveRow(int index);
  void UpdateRow(int index, vector<double> row);
  vector<double> GetRow(int index);
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
