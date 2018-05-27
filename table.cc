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


#endif

#if defined(DISTANCEVECTOR)

Table::Table()
{}

Table::Table(int sz)
{
  vector<vector<double> > matrix(sz);
  for ( int i = 0 ; i < sz ; i++ )
     matrix[i].resize(sz);
   this.matrix = matrix;
   // this.prev = matrix;
   this.sz = sz;
}

ostream & Table::Print(ostream &os) const
{
  char[] string = "";

  for (int i = 0; i < this.sz ; i++) {
    for (int j = 0; j < this.sz ; j++) {
      string = strcat(string, this.matrix[i][j]);
      string = strcat(string, " ");
    }
    string = strcat(string, "\n");
  }
  string = strcat(string, "\n\n");


  os << string;
  return os;
}

void Table::AddRow();
{
  vector<double> newrow(sz);
  // add the rows
  this.matrix.pushback(newrow);
  for ( int i = 0 ; i < sz+1 ; i++ )
  {
    // we add the columns now
    this.matrix[i].pushback(0.0);
  }
  this.sz++;
  return;
}

void Table::RemoveRow(int index);
{
  this.matrix.erase(this.matrix.begin()+index);
  for ( int i = 0 ; i < sz - 1 ; i++ )
  {
    this.matrix[i].erase(this.matrix[i].begin()+index);
  }
  return;
}

void Table::UpdateRow(int index, vector<double> row);
{
  this.matrix[index] = row;
  // this.prev[index] = prev;
}

vector<double> Table::GetRow(int index);
{
  return this.matrix[index];
}


#endif
