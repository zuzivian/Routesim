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
    os << "\nnext hop\n";
    for (int i = 0; i < this.sz ; i++) {
      os << this.next_hop[i] << " ";
    }
    os << "\n";
    return os;
  }

  bool Table::ComputeMatrix()
  {
    //Implement DV
    inf = std::numeric_limits<double>::infinity();
    vector<double> best = vector<double> (this.size, inf);
    vector<unsigned> best_hop = vector<unsigned> (this.size, this.index);
    bool updated = false;
    // loop through matrix vectors and update link cost
    for (int i = 0; i < this.size(); i++)
    {
      if (i == this.index) continue; // don't process own row
      for (int j = 0; j < this.size(); j++)
      {
        // process neighbors only
        if (j == this.index || isinf(link_cost[j])) continue;
        // take minimum of current value and new value
        double new_cost = this.link_cost[j] + this.matrix[j][i];
        if (new_cost < best[i]) {
          // update best_dist
          best[i] = new_cost;
          best_hop[i] = j;
        }
      }
      if (matrix[this.index][i] != best[i]) updated = true;
      matrix[this.index][i] = best[i];
      next_hop[i] = best_hop[i];
    }
    return updated;
  }

  // Takes a distance vector from neighbor and add to matrix, then update matrix.
  // returns true if matrix was updated
  bool Table::UpdateMatrix(unsigned index, vector<double> vec) {
    if (index != this.index) return false;
    if (vec.size() != this.size) return false;
    this.matrix[index] = vec;
    return ComputeMatrix();
  }

  // Takes a link cost from neighbor and add to cost vector, then update matrix.
  // returns true if matrix was updated
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
