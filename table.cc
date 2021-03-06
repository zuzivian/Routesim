#include "table.h"
#include "error.h"
#include <algorithm>

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

Table::Table(unsigned ind)
{
  this->index = ind;
  this->pred = map<unsigned, unsigned> ();
  this->t = map<unsigned, map<unsigned, Link> > ();
  this->id = map<unsigned, map<unsigned, unsigned> > ();
}

Table::Table(const Table &rhs) :
  index(rhs.index), pred(rhs.pred) {}


ostream & Table::Print(ostream &os) const
{
  os << "dist: ";
  for(map<unsigned, double>::const_iterator it=dist.begin(); it!=dist.end(); ++it)
    os << it->first << ":" << it->second << ";  ";
  os << endl << "pred: ";
  for(map<unsigned, unsigned>::const_iterator it=pred.begin(); it!=pred.end(); ++it)
    os << it->first << ":" << it->second << ";  ";
  os << "\n";
  return os;
}



bool Table::ComputeDijkstra()
{
  // Q holds queue of vertices
  map<unsigned, map<unsigned, Link> > Q;
  map<unsigned, map<unsigned, Link> >::iterator it;

  for (it = t.begin(); it != t.end(); it++)
  {
    dist[it->first] = std::numeric_limits<double>::infinity();
    pred[it->first] = index;
    Q[it->first] = it->second;
  }
  dist[index] = 0;
  while (Q.size() != 0)
  {
    double smallest = std::numeric_limits<double>::infinity();
    unsigned smallest_node = Q.begin()->first;
    for (it = Q.begin(); it != Q.end(); it++)
    {
      if (dist[it->first] < smallest)
      {
        smallest = dist[it->first];
        smallest_node = it->first;
      }
    }
    Q.erase(Q.find(smallest_node));
    // iterate through all nodes
    for (it = t.begin(); it != t.end(); it++)
    {
      // if node is a neighbor, check distance
      if (t.count(smallest_node) && t[smallest_node].count(it->first)) {
        double alt = dist[smallest_node] + t[smallest_node][it->first].GetLatency();
        if (alt < dist[it->first]) {
          dist[it->first] = alt;
          pred[it->first] = smallest_node;
        }
      }
    }
  }
  return true;
}


bool Table::UpdateLink(const Link l)
{
  // check if node is new
  unsigned src = l.GetSrc();
  unsigned dest = l.GetDest();
  if (t.count(dest) == 0) t[dest]; // check if link is new
  if (t[src].count(dest) == 0) {
    cerr << "New link: " << l << endl;
    t[src][dest] = l;
    id[src][dest] = 2;
  }
  else
  {
    cerr << "Link update accepted: " << l << endl;
    t[src][dest] = l;
    id[src][dest]++;
  }
  // get new table
  ComputeDijkstra();
  return true;
}

bool Table::UpdateMessageLink(unsigned nid, unsigned sender, const Link l)
{
  unsigned src = l.GetSrc();
  unsigned dest = l.GetDest();
  if (t.count(src) == 0) t[src]; // check if src is new
  if (t.count(dest) == 0) t[dest]; // check if dest is new
  // update only if is new link, or t.id != id AND
  if (t[src].count(dest) == 0 || nid > this->id[src][dest])
  {
    cerr << "Link update accepted!: " << l << " nid: " << nid << " vs current id: " << this->id[src][dest] << endl;
    this->t[src][dest] = l;
    this->id[src][dest] = nid;
    ComputeDijkstra();
    return true;
  }
  else
  {
    cerr << "Link update discarded: " << l << " nid: " << nid << " vs current id: " << this->id[src][dest] << endl;
    return false;
  }
}

unsigned Table::GetLinkID(unsigned src, unsigned dest)
{
  return this->id[src][dest];
}

unsigned Table::GetNextHop(unsigned ind)
{
  unsigned next_hop = ind;
  for (unsigned i = ind; i != index; i = pred[i])
    next_hop = i;
  return next_hop;
}


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
    bool updated = false;
    // loop through matrix vectors and update link cost
    for (unsigned i = 0; i < this->size; i++)
    {
      double best = inf;
      unsigned best_hop = this->index;
      if (i == this->index) continue; // don't process own row
      for (unsigned j = 0; j < this->size; j++)
      {
        // process neighbors only
        if (j == this->index) continue;
        // take minimum of current value and new value
        double new_cost = this->link_cost[j] + this->matrix[j][i];
        if (new_cost < best) {
          // update best_dist
          best = new_cost;
          best_hop = j;
        }
      }
      if (this->matrix[this->index][i] != best) updated = true;
      this->matrix[this->index][i] = best;
      this->next_hop[i] = best_hop;
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
  std::vector<double> Table::GetVector() const
  {
    return matrix[this->index];
  }


#endif
