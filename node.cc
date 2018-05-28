#include "node.h"
#include "context.h"
#include "error.h"

Node::Node(const unsigned n, SimulationContext *c, double b, double l) :
    number(n), context(c), bw(b), lat(l)
{
  #if defined(DISTANCEVECTOR)
  this->tbl = Table(n, 25);
  #endif
}

Node::Node()
{ throw GeneralException(); }

Node::Node(const Node &rhs) :
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}

Node & Node::operator=(const Node &rhs)
{
  return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n)
{ number=n;}

unsigned Node::GetNumber() const
{ return number;}

void Node::SetLatency(const double l)
{ lat=l;}

double Node::GetLatency() const
{ return lat;}

void Node::SetBW(const double b)
{ bw=b;}

double Node::GetBW() const
{ return bw;}

Node::~Node()
{}

// Implement these functions  to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m)
{
  deque<Node*> neighbors = *this->GetNeighbors();
  for (unsigned i=0; i < neighbors.size(); i++)
  {
    SendToNeighbor(neighbors[i], m);
  }
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
  Link x = Link(this->GetNumber(),n->GetNumber(),0,0,0);
  Link *l = context->FindMatchingLink(&x);

  context->PostEvent(new Event(context->GetTime()+l->GetLatency(),
		      ROUTING_MESSAGE_ARRIVAL,
		      context->FindMatchingNode(n),
		      (void*)m));
}

deque<Node*> *Node::GetNeighbors()
{
  return context->GetNeighbors(this);
}

void Node::SetTimeOut(const double timefromnow)
{
  context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
  return number==rhs.number;
}


#if defined(GENERIC)
void Node::LinkHasBeenUpdated(const Link *l)
{
  cerr << *this << " got a link update: "<<*l<<endl;
  //Do Something generic:
  SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  return 0;
}

Table *Node::GetRoutingTable() const
{
  return new Table;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}

#endif

#if defined(LINKSTATE)


void Node::LinkHasBeenUpdated(const Link *l)
{
  cerr << *this<<": Link Update: "<<*l<<endl;
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " Routing Message: "<<*m;
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  // WRITE
  return 0;
}

Table *Node::GetRoutingTable() const
{
  // WRITE
  return 0;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}
#endif


#if defined(DISTANCEVECTOR)

void Node::LinkHasBeenUpdated(const Link *l)
{
  // this is an outgoing vector
  // update our table
  // send out routing mesages
  cerr << *this<<": Link Update: "<<*l<<endl;
  double newlat = l->GetLatency();
  unsigned dest = l->GetDest();
  unsigned src = l->GetSrc();

  if (src != this->GetNumber())
  {
    cerr << *this<<": Invalid Link!"<<endl;
    return; // invalid link

  }
  bool updated = this->tbl.UpdateLink(dest, newlat);
  if (updated)
  {
    //cout << this->tbl.Print(cout);
    std::vector<double> new_vec = this->tbl.GetVector(-1);
    const RoutingMessage* routing_message = new RoutingMessage(src, new_vec);
    // send messages to all neighbors
    SendToNeighbors(routing_message);
  }
  else
  {
    cerr << *this <<": No update!"<<endl;
  }
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  unsigned src = m->sender;
  std::vector<double> new_vec = m->dist_vec;
  cerr << *this <<": Received message from "<<src<<endl;
  cerr << m->Print(cerr)<<endl;

  bool updated = this->tbl.UpdateMatrix(src, new_vec);
  if (updated)
  {
    cout << this->tbl.Print(cout);
    std::vector<double> new_vec = this->tbl.GetVector(-1);
    const RoutingMessage* routing_message = new RoutingMessage(src, new_vec);
    // send messages to all neighbors
    SendToNeighbors(routing_message);
  }
  else
  {
    cerr << *this << ": No updates to table." << endl;
  }

}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}


Node *Node::GetNextHop(const Node *destination)
{
  // returns a Node*
  unsigned num = this->GetNumber();
  unsigned nexthop = this->tbl.GetNextHop(destination->GetNumber());
  if (nexthop == num) return NULL; // infinite loop
  deque<Node*> neighbors = *this->GetNeighbors();

  for (unsigned i = 0; i < neighbors.size() ; i++)
  {
      if (nexthop == neighbors[i]->GetNumber())
        return new Node(*neighbors[i]);
  }
  throw GeneralException();
}

Table *Node::GetRoutingTable() const
{
  return new Table(this->tbl);
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
  return os;
}
#endif
