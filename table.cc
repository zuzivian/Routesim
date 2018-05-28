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

  Table::Table(unsigned ind, unsigned sz)
  {
    double inf = std::numeric_limits<double>::infinity();
    this->index = ind;
    this->size = sz;
    // initialize all next_hops as unreachable
    this->next_hop = std::vector<unsigned> (sz, ind);
    this->link_cost = std::vector<double> (sz, inf);
    this->matrix = std::vector<std::vector<double> >(sz, std::vector<double>(sz, inf));
    for (unsigned i = 0; i < sz; i++)
      matrix[i][i] = 0.0;
  }

  Table::Table(const Table &rhs) :
    size(rhs.size), index(rhs.index), next_hop(rhs.next_hop),
    link_cost(rhs.link_cost), matrix(rhs.matrix)
    {}

  ostream & Table::Print(ostream &os) const
  {
    os << "matrix\n";
    for (unsigned i = 0; i < this->size ; i++) {
      for (unsigned j = 0; j < this->size ; j++) {
        os << this->matrix[i][j] << " ";
      }
      os << "\n";
    }
    os << "\nlink cost\n";
    for (unsigned i = 0; i < this->size ; i++) {
      os << this->link_cost[i] << " ";
    }
    os << "\nnext hop\n";
    for (unsigned i = 0; i < this->size ; i++) {
      os << this->next_hop[i] << " ";
    }
    os << "\n";
    return os;
  }

  bool Table::ComputeMatrix()
  {
    //Implement DV
    double inf = std::numeric_limits<double>::infinity();
    std::vector<double> best = std::vector<double> (this->size, inf);
    std::vector<unsigned> best_hop = std::vector<unsigned> (this->size, this->index);
    bool updated = false;
    // loop through matrix vectors and update link cost
    for (unsigned i = 0; i < this->size; i++)
    {
      if (i == this->index) continue; // don't process own row
      for (unsigned j = 0; j < this->size; j++)
      {
        // process neighbors only
        if (j == this->index || std::isinf(link_cost[j])) continue;
        // take minimum of current value and new value
        double new_cost = this->link_cost[j] + this->matrix[j][i];
        if (new_cost < best[i]) {
          // update best_dist
          best[i] = new_cost;
          best_hop[i] = j;
        }
      }
      cerr << best[i] << "vs"<< matrix[this->index][i] << endl;
      if (matrix[this->index][i] != best[i]) updated = true;
      matrix[this->index][i] = best[i];
      next_hop[i] = best_hop[i];
    }
    return updated;
  }

  // Takes a distance vector from neighbor and add to matrix, then update matrix.
  // returns true if matrix was updated
  bool Table::UpdateMatrix(unsigned ind, std::vector<double> vec) {
    if (ind == this->index) {
      cerr << *this <<": Cannot update own vector!"<<endl;
      return false;
    }
    if (vec.size() != this->size) {
      cerr << *this <<": Invalid vector length!"<<endl;
      return false;
    }
    this->matrix[ind] = vec;
    return ComputeMatrix();
  }

  // Takes a link cost from neighbor and add to cost vector, then update matrix.
  // returns true if matrix was updated
  bool Table::UpdateLink(unsigned ind, double newcost)
  {
    if (ind == this->index)
    {
      cerr << *this <<": Cannot update own node cost!"<<endl;
      return false;
    }
    this->link_cost[ind] = newcost;
    return ComputeMatrix();
  }

  unsigned Table::GetNextHop(unsigned ind) const
  { return this->next_hop[ind]; }

  // use GetVector(-1) to get own vector
  std::vector<double> Table::GetVector(int ind) const
  {
    if (ind < 0 ) return matrix[this->index];
    return matrix[(unsigned)ind];
  }


#endif
