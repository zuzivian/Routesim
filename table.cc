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

  Table::Table() {}

  Table(unsigned ind, unsigned sz)
  {
    assert(ind <= sz);
    double inf = std::numeric_limits<double>::infinity();
    this.index = ind;
    this.size = sz;
    // initialize all next_hops as unreachable
    this.next_hop = vector<unsigned> (sz, ind);
    this.link_cost = vector<double> (sz, inf);
    this.matrix = vector<vector<double>>(sz, vector<double>(sz, inf));
    for (int i = 0; i < sz; i++)
      matrix[i][i] = 0.0;
  }

  Table(const Table &rhs) :
    index(rhs.index), size(rhs.size), next_hop(rhs.next_hop),
    link_cost(rhs.link_cost), matrix(rhs.matrix)
    {}

  ostream & Table::Print(ostream &os) const
  {
    os << "matrix\n";
    for (int i = 0; i < this.sz ; i++) {
      for (int j = 0; j < this.sz ; j++) {
        os << this.matrix[i][j] << " ";
      }
      os << "\n";
    }
    os << "\nlink cost\n");
    for (int i = 0; i < this.sz ; i++) {
      os << this.link_cost[i] << " ";
    }
    os << "\nnext hops\n";
    for (int i = 0; i < this.sz ; i++) {
      os << this.next_hop[i] << " ";
    }
    os << "\n";
    return os;
  }

  bool Table::ComputeMatrix()
  {
    //TODO: Implement DV
    return false;
  }

  // Takes a distance vector from neighbor and add to matrix, then update matrix.
  bool Table::UpdateMatrix(unsigned index, vector<double> vec) {
    if (index != this.index) return false;
    if (vec.size != this.size) return false;
    this.matrix[index] = vec;
    return ComputeMatrix();
  }

  // Takes a link cost from neighbor and add to cost vector, then update matrix.
  bool Table::UpdateLink(unsigned index, double newcost)
  {
    if (index != this.index) return false;
    this.link_cost[index] = newcost;
    return ComputeMatrix();
  }

  unsigned Table::GetNextHop(unsigned index)
  { return this.next_hop(index); }

  // use GetVector(-1) to get own vector
  vector<double> Table::GetVector(int index)
  {
    if (index < 0 ) return matrix[this.index];
    return matrix[(unsigned)index];
  }


#endif
