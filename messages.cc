#include "messages.h"
#include "error.h"

#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "Message from node " << this->sender << ", id " << this->id;
  os << ", about "<< this->link << "\n";
  return os;
}

RoutingMessage::RoutingMessage()
{
  cerr << "RoutingMessage: empty message\n";
  throw GeneralException();
}

RoutingMessage::RoutingMessage(unsigned id, unsigned src, Link l)
{
  this->id = id;
  this->sender = src;
  this->link = l;
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) :
  id(rhs.id), sender(rhs.sender), link(rhs.link) {}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "Message from node " << this->sender << ":\n";
  for (unsigned i = 0; i < this->dist_vec.size(); i++)
    os << this->dist_vec[i] << " ";
  os << "\n";
  return os;
}

RoutingMessage::RoutingMessage()
{
  cerr << "RoutingMessage: empy message\n";
  throw GeneralException();
}

RoutingMessage::RoutingMessage(unsigned src, std::vector<double> vec)
{
  this->dist_vec = vec;
  this->sender = src;
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) :
  dist_vec(rhs.dist_vec), sender(rhs.sender) {}

#endif
