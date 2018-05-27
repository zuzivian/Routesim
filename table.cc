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

Table::Table(int sz)
{


}

ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
void Table::AddRow();
{

}

void Table::RemoveRow(int index);
{

}

void Table::UpdateRow(int index, vector<double> dist, vector<double> prev);
{

}

void Table::GetRow(int index);
{

}


#endif

#if defined(DISTANCEVECTOR)


#endif
