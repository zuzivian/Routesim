#ifndef _messages
#define _messages

#include <iostream>

#include "node.h"
#include "link.h"

#if defined(GENERIC)
class RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
class RoutingMessage {
 public:
  unsigned id;
  unsigned sender;
  const Link *link;

  RoutingMessage();
  RoutingMessage(unsigned id, unsigned src, const Link l);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
class RoutingMessage {
 public:
  std::vector<double> dist_vec;
  unsigned sender;
  RoutingMessage();
  RoutingMessage(unsigned src, std::vector<double> vec);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
