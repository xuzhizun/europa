//  Copyright Notices

//  This software was developed for use by the U.S. Government as
//  represented by the Administrator of the National Aeronautics and
//  Space Administration. No copyright is claimed in the United States
//  under 17 U.S.C. 105.

//  This software may be used, copied, and provided to others only as
//  permitted under the terms of the contract or other agreement under
//  which it was acquired from the U.S. Government.  Neither title to nor
//  ownership of the software is hereby transferred.  This notice shall
//  remain on all copies of the software

#include "Edge.hh"
#include "EdgeIterator.hh"
#include "Graph.hh"
#include "Node.hh"
#include "NodeIterator.hh"

namespace EUROPA
{
Graph::Graph() : m_Nodes() {}

    Graph::~Graph()
    {
      {
	NodeIdentity2Node::iterator ite = m_Nodes.begin();
	NodeIdentity2Node::iterator end = m_Nodes.end();

	for( ; ite != end; ++ite )
	  {
	    delete (*ite).second;
	  }
      }
    }

    Node* Graph::getNode( const NodeIdentity& identity ) const
    {
      Node* node = 0;

      NodeIdentity2Node::const_iterator ite = m_Nodes.find( identity );

      if( m_Nodes.end() != ite )
	node = (*ite).second;

      return node;
    }

    Node* Graph::createNode( const NodeIdentity& identity, bool enabled )
    {
      Node* node = getNode( identity );

      if( 0 == node )
	{

	  node = new Node( identity );

	  graphDebug("Created node "
		     << *node << " initially " << (enabled ? "enabled" : "disabled"));

	  m_Nodes[ identity ] = node;
	}

      if( enabled ) {
	node->setEnabled();
      }
      else {
	node->setDisabled();
      }

      return node;
    }

    void Graph::removeNode( const NodeIdentity& identity )
    {
      NodeIdentity2Node::iterator ite = m_Nodes.find( identity );

      if( m_Nodes.end() != ite )
	{
	  Node* node = (*ite).second;

	  m_Nodes.erase( ite );

	  graphDebug("Removed node "
		     << *node );

	  delete node;
	}
    }


    void Graph::createEdge( const NodeIdentity& source, const NodeIdentity& target, edouble capacity, bool enabled )
    {
      Node* sourceNode = createNode( source );
      Node* targetNode = createNode( target );

      if( 0 != sourceNode && 0 != targetNode )
	{
	  createEdge( sourceNode, targetNode, capacity, enabled );
	}
    }

    Edge* Graph::createEdge( Node* source, Node* target, edouble capacity, bool enabled )
    {
      checkError( 0 != source, "Null not allowed as input for source");
      checkError( 0 != target, "Null not allowed as input for target" );

      Edge* edge = getEdge( source, target );

      if( 0 == edge )
	{
	  edge = new Edge( source, target, capacity, enabled );

	  graphDebug("Created edge "
		     << *edge << " with capacity "
		     << capacity );

	}
      else
	{
	  graphDebug("Setting capacity of edge "
		     << *edge << " to be {"
		     << capacity << "}");

	  if( enabled )
	    edge->setEnabled();

	  edge->setCapacity( capacity );
	}

      return edge;
    }

    void Graph::setDisabled()
    {
      graphDebug("Disabling whole graph.")
      NodeIdentity2Node::iterator ite = m_Nodes.begin();
      NodeIdentity2Node::iterator end = m_Nodes.end();

      for( ; ite != end; ++ite )
	{
	  Node* node = (*ite).second;

	  node->setDisabled();

	  EdgeOutIterator outIte( *node );

	  for( ; outIte.ok(); ++outIte )
	    {
	      Edge* edge = (*outIte);

	      edge->setDisabled();
	      // we do not have todo the in-edges because we go
	      // over all nodes
	    }
	}
    }
}
