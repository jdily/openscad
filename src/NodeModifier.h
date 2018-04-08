#pragma once

#include "BaseVisitable.h"
#include "node.h"
#include "state.h"


class NodeModifier : public BaseModifier, 
    public Modifier<class AbstractNode>
{

public:
    NodeModifier();
    ~NodeModifier();

    Response traverse(AbstractNode &node, const class State &state = NodeModifier::nullstate);
    Response visit(class State &state, class AbstractNode &node) = 0;

private:
    static State nullstate;
};