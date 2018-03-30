#pragma once

#include "nodecache.h"

/*!  
	For now, just an abstraction of the node tree which keeps a dump
	cache based on node indices around.

	Note that since node trees don't survive a recompilation, the tree cannot either.
 */
class Tree
{
public:
	Tree(const AbstractNode *root = nullptr) : root_node(root) {}
	~Tree();

	// ichao : add copy constructor
	Tree(const Tree &obj);

	void setRoot(const AbstractNode *root);
	const AbstractNode *root() const { return this->root_node; }

	const std::string &getString(const AbstractNode &node) const;
	const std::string &getIdString(const AbstractNode &node) const;

	// get the height of the node
	// int height(AbstractNode &node);
	// 
	int node_count();
	// manipulation operators
	void remove_node(int idx);
	void remove_node(AbstractNode *node);
	
	bool contains(AbstractNode *node) {
		return nodecache.contains(*node);
	}

	NodeCache& get_cache() { return this->nodecache; } 	
	void clear_cache();
private:
  const AbstractNode *root_node;
  mutable NodeCache nodecache;
  mutable NodeCache nodeidcache;
};
