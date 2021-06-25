#pragma once

#include <QtCore/QUuid>

#include "PortType.hpp"

class QPointF;

namespace QtNodes
{

class Node;

/// Stores currently draggind end.
/// Remembers last hovered Node.
class ConnectionState
{
public:

  ConnectionState(PortType port = PortType::None)
    : _requiredPort(port),
	  _preview(false)  // MK : manage preview for connections
  {}

  ConnectionState(const ConnectionState&) = delete;
  ConnectionState operator=(const ConnectionState&) = delete;

  ~ConnectionState();

public:

  void setRequiredPort(PortType end)
  { _requiredPort = end; }

  PortType requiredPort() const
  { return _requiredPort; }

  bool requiresPort() const
  { return _requiredPort != PortType::None; }

  void setNoRequiredPort()
  { _requiredPort = PortType::None; }

  // MK : manage preview for connections
  void setIsAPreview(bool isAPreview)
  { _preview = isAPreview; }

  bool isAPreview() const
  { return _preview; }

public:

  void interactWithNode(Node* node);

  void setLastHoveredNode(Node* node);

  Node*
  lastHoveredNode() const
  { return _lastHoveredNode; }

  void resetLastHoveredNode();

private:

  PortType _requiredPort;
  bool	   _preview;  // MK : manage preview for connections

  Node* _lastHoveredNode{nullptr};
};
}
