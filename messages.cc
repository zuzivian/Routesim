#include "messages.h"


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
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "Message from node " << this.sender <, ":\n";
  for (int i = 0; i < this.dist_vec.size(); i++)
    os << this.dist_vec[i] << " ";
  os << "\n";
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage(unsigned sender, vector<double> vec)
{
  this.dist_vec = vec;
  this.sender = sender;
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) :
  dist_vec(rhs.dist_vec) sender(rhs.sender) {}

#endif
