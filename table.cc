#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

Table::Table()
{}

Table::Table(int sz)
{
  vector<vector<double> > matrix(sz);
  for ( int i = 0 ; i < sz ; i++ )
     matrix[i].resize(sz);
   this.dist = matrix;
   this.prev = matrix;
   this.sz = sz;
}

ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
void Table::AddRow();
{
  vector<double> newrow(sz);
  // add the rows
  this.dist.pushback(newrow);
  this.prev.pushback(newrow);
  for ( int i = 0 ; i < sz+1 ; i++ )
  {
    // we add the columns now
    this.dist[i].pushback(0.0);
    this.prev[i].pushback(0.0);
  }
  this.sz++;
  return;
}

void Table::RemoveRow(int index);
{
  this.dist.erase(this.dist.begin()+index);
  this.prev.erase(this.prev.begin()+index);
  for ( int i = 0 ; i < sz - 1 ; i++ )
  {
    this.dist[i].erase(this.dist[i].begin()+index);
    this.prev[i].erase(this.prev[i].begin()+index);
  }
  return;
}

void Table::UpdateRow(int index, vector<double> dist, vector<double> prev);
{
  this.dist[index] = dist;
  this.prev[index] = prev;
}

vector<double> Table::GetDistRow(int index);
{
  return this.dist[index];
}


vector<double> Table::GetPrevRow(int index);
{
  return this.prev[index];
}

#endif

#if defined(DISTANCEVECTOR)


#endif
