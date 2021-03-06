#include "node.h"
#include "context.h"
#include "error.h"

Node::Node(const unsigned n, SimulationContext *c, double b, double l) :
    number(n), context(c), bw(b), lat(l)
{
  #if defined(DISTANCEVECTOR)
    this->tbl = Table(n, 25);
  #endif

  #if defined(LINKSTATE)
    this->tbl = Table(n);
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
  if (l->GetSrc() != this->GetNumber()) return; // invalid link

  if (this->tbl.UpdateLink(*l))
  {
    cout << *this << ": Link updated: " << *l <<" Sending message to neighbors." << endl;
    this->tbl.Print(cout);
    // send messages to all neighbors
    unsigned id =  this->tbl.GetLinkID(l->GetSrc(), l->GetDest());
    const RoutingMessage* m = new RoutingMessage(id, number, *l);
    SendToNeighbors(m);
  }
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cout << *this <<": Received message." <<endl;
  m->Print(cout);

  if (this->tbl.UpdateMessageLink(m->id, m->sender, m->link))
  {
    cout << *this << ": Table updated. Forwarding message to neighbors." << endl;
    this->tbl.Print(cout);
    // send messages to all neighbors
    SendToNeighbors(m);
  }
}

void Node::TimeOut()
{
  cout << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination)
{
  // returns a Node*
  unsigned num = this->GetNumber();
  unsigned nexthop = this->tbl.GetNextHop(destination->GetNumber());
  if (nexthop == num) return NULL; // infinite loop
  deque<Node*> neighbors = *GetNeighbors();

  for (unsigned i = 0; i < neighbors.size() ; i++)
  {
      if (nexthop == neighbors[i]->GetNumber())
        return new Node(*neighbors[i]);
  }
  return NULL;
}

Table *Node::GetRoutingTable() const
{
  return new Table(this->tbl);
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

  if (this->tbl.UpdateLink(dest, newlat))
  {
    //cout << this->tbl.Print(cout);
    std::vector<double> my_vec = this->tbl.GetVector();
    const RoutingMessage* routing_message = new RoutingMessage(number, my_vec);
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

  if (this->tbl.UpdateMatrix(src, new_vec))
  {
    cout << this->tbl.Print(cout);
    std::vector<double> my_vec = this->tbl.GetVector();
    const RoutingMessage* routing_message = new RoutingMessage(number, my_vec);
    // send messages to all neighbors
    SendToNeighbors(routing_message);
  }
  else
  {
    cerr << *this << ": No updates to table." << endl;
    cout << this->tbl.Print(cout);
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
  deque<Node*> neighbors = *GetNeighbors();

  for (unsigned i = 0; i < neighbors.size() ; i++)
  {
      if (nexthop == neighbors[i]->GetNumber())
        return new Node(*neighbors[i]);
  }
  return NULL;
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
