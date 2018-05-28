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
    std::vector<double> best = std::vector<double> (size, inf);
    std::vector<unsigned> best_hop = std::vector<unsigned> (size, index);
    bool updated = false;
    // loop through matrix vectors and update link cost
    for (unsigned i = 0; i < size; i++)
    {
      if (i == index) continue; // don't process own row
      for (unsigned j = 0; j < size; j++)
      {
        // process neighbors only
        if (j == index || std::isinf(link_cost[j])) continue;
        double new_cost = link_cost[j] + matrix[j][i];
        if (new_cost < best[i]) {
          // if better, update
          best[i] = new_cost;
          best_hop[i] = j;
        }
      }
      if (matrix[index][i] != best[i]) updated = true;
      matrix[index][i] = best[i];
      next_hop[i] = best_hop[i];
    }
    return updated;
  }

  // Takes a distance vector from neighbor and add to matrix, then update matrix.
  // returns true if matrix was updated
  bool Table::UpdateMatrix(unsigned ind, std::vector<double> vec) {
    if (ind == index) {
      cerr << *this <<": Cannot update own vector!"<<endl;
      return false;
    }
    if (vec.size() != size) {
      cerr << *this <<": Invalid vector length!"<<endl;
      return false;
    }
    matrix[ind] = vec;
    return ComputeMatrix();
  }

  // Takes a link cost from neighbor and add to cost vector, then update matrix.
  // returns true if matrix was updated
  bool Table::UpdateLink(unsigned ind, double newcost)
  {
    if (ind == index)
    {
      cerr << *this <<": Cannot update own node cost!"<<endl;
      return false;
    }
    link_cost[ind] = newcost;
    return ComputeMatrix();
  }

  unsigned Table::GetNextHop(unsigned ind) const
  { return next_hop[ind]; }

  // use GetVector(-1) to get own vector
  std::vector<double> Table::GetVector(int ind) const
  {
    if (index < 0 ) return matrix[index];
    return matrix[(unsigned)ind];
  }


#endif
