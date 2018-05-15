#include "CSGTreeEvaluator.h"
#include "state.h"
#include "csgops.h"
#include "module.h"
#include "ModuleInstantiation.h"
#include "csgnode.h"
#include "transformnode.h"
#include "colornode.h"
#include "rendernode.h"
#include "cgaladvnode.h"
#include "printutils.h"
#include "GeometryEvaluator.h"
#include "polyset.h"
#include "polyset-utils.h"

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <cstddef>
#include <Tree.h>

/*!
	\class CSGTreeEvaluator

	A visitor responsible for creating a binary tree of CSGNode nodes used for rendering
	with OpenCSG.
*/

shared_ptr<CSGNode> CSGTreeEvaluator::buildCSGTree(const AbstractNode &node)
{
	std::cout << "CSGTreeEvaluator buildCSGTree" << std::endl;
	this->traverse(node);
	check_stored_term();
	// std::cout << "node index : " << node.index() << std::endl;
	shared_ptr<CSGNode> t(this->stored_term[node.index()]);
	if (t) {
		if (t->isHighlight()) this->highlightNodes.push_back(t);
		if (t->isBackground()) {
			this->backgroundNodes.push_back(t);
			t.reset();
		}
	}
	return this->rootNode = t;
}

// build the CSG tree with manually assigned highlight geometries..
// The purpose is to visualize the relationship between tree nodes and the geometries...
shared_ptr<class CSGNode> CSGTreeEvaluator::buildCSGTree_w_hb(const AbstractNode &node, std::vector<int> hids) {
	this->traverse(node);
	
	check_stored_term();
	this->stored_term[1]->setHighlight(false);
	for (int i = 0; i < (int)hids.size(); i++) {
		std::cout << hids[i] << std::endl;
		this->stored_term[hids[i]]->setHighlight(true);
	}
	check_stored_term();
	// for (int i = 0; i < this->stored_term.size(); i++) {
	// 	if (std::find(hids.begin(), hids.end(), i) == hids.end()) {
	// 		this->stored_term[i]->setBackground(true);
	// 	}
	// }
	shared_ptr<CSGNode> t(this->stored_term[node.index()]);
	if (t) {
		if (t->isHighlight()) this->highlightNodes.push_back(t);
		// TODO : check if this helps...
		// if (t->isBackground()) {
		// 	this->backgroundNodes.push_back(t);
		// 	t.reset();
		// }
	}
	std::cout << "number of highlight : " << this->highlightNodes.size() << std::endl;
	std::cout << "number of background : " << this->backgroundNodes.size() << std::endl;
	return this->rootNode = t;
}

void CSGTreeEvaluator::check_stored_term() {
	// go through all keys
	std::cout << "check stored term" << std::endl;
	std::cout << "stored size : " << stored_term.size() << std::endl;

	std::cout << "number of highlight : " << this->highlightNodes.size() << std::endl;
	std::cout << "number of background : " << this->backgroundNodes.size() << std::endl;
	for (int i = 1; i < (int)this->stored_term.size()+1; i++) {
		if (this->stored_term[i]->isHighlight()) {
			std::cout << i << " is highlighted " << std::endl;
		} else {
			std::cout << i << " is background " << std::endl;
		}
	}

	// manual change one of them as highlight
	// this->stored_term[6]->setHighlight(true);

	// std::vector<int> v;
	// for(std::map<int,shared_ptr<CSGNode>>::iterator it = stored_term.begin(); it != stored_term.end(); ++it) {
  	// 	v.push_back(it->first);
  	// 	std::cout << it->first << "  " << it->second->dump() << std::endl;
	// }
}


void CSGTreeEvaluator::applyBackgroundAndHighlight(State & /*state*/, const AbstractNode &node)
{
	for(const auto &chnode : this->visitedchildren[node.index()]) {
		shared_ptr<CSGNode> t(this->stored_term[chnode->index()]);
		// ichao : check if we can retain all these stored_term
		// this->stored_term.erase(chnode->index());
		if (t) {
			if (t->isBackground()) this->backgroundNodes.push_back(t);
			if (t->isHighlight()) this->highlightNodes.push_back(t);
		}
	}
}

// ichao : trace here...
void CSGTreeEvaluator::applyToChildren(State & state, const AbstractNode &node, OpenSCADOperator op)
{
	shared_ptr<CSGNode> t1;
	for(const auto &chnode : this->visitedchildren[node.index()]) {
		shared_ptr<CSGNode> t2(this->stored_term[chnode->index()]);
		// ichao : check if we can retain all these stored_term
		// this->stored_term.erase(chnode->index());
		if (t2 && !t1) {
			t1 = t2;
		} else if (t2 && t1) {

			shared_ptr<CSGNode> t;
			// Handle background
			if (t1->isBackground() && 
					// For difference, we inherit the flag from the positive object
					(t2->isBackground() || op == OpenSCADOperator::DIFFERENCE)) {
				t = CSGOperation::createCSGNode(op, t1, t2);
				t->setBackground(true);
			}
			// Background objects are simply moved to backgroundNodes
			else if (t2->isBackground()) {
				t = t1;
				this->backgroundNodes.push_back(t2);
			}
			else if (t1->isBackground()) {
				t = t2;
				this->backgroundNodes.push_back(t1);
			}
			else {
				t = CSGOperation::createCSGNode(op, t1, t2);
			}
			// Handle highlight
				switch (op) {
				case OpenSCADOperator::DIFFERENCE:
					if (t != t1 && t1->isHighlight()) {
						t->setHighlight(true);
					}
					else if (t != t2 && t2->isHighlight()) {
						this->highlightNodes.push_back(t2);
					}
					break;
				case OpenSCADOperator::INTERSECTION:
					if (t && t != t1 && t != t2 &&
							t1->isHighlight() && t2->isHighlight()) {
						t->setHighlight(true);
					}
					else {
						if (t != t1 && t1->isHighlight()) {
							this->highlightNodes.push_back(t1);
						}
						if (t != t2 && t2->isHighlight()) {
							this->highlightNodes.push_back(t2);
						}
					}
					break;
				case OpenSCADOperator::UNION:
					if (t != t1 && t != t2 &&
							t1->isHighlight() && t2->isHighlight()) {
						t->setHighlight(true);
					}
					else if (t != t1 && t1->isHighlight()) {
						this->highlightNodes.push_back(t1);
						t = t2;
					}
					else if (t != t2 && t2->isHighlight()) {
						this->highlightNodes.push_back(t2);
						t = t1;
					}
					break;
                case OpenSCADOperator::MINKOWSKI:
                case OpenSCADOperator::HULL:
                case OpenSCADOperator::RESIZE:
                    break;
				}
			t1 = t;
		}
	}
	if (t1) {
		if (node.modinst->isBackground()) t1->setBackground(true);
		if (node.modinst->isHighlight()) t1->setHighlight(true);
	}
	this->stored_term[node.index()] = t1;
	// ichao added -> added more geometries...
	// make new geometry evaluation for all leaf...
	// shared_ptr<CSGNode> gt;
	// Tree subtree;
	// subtree.setRoot(&node);
	// GeometryEvaluator geomevaluator(subtree);
	// std::cout << "preflag " << std::endl;
	// auto geom = this->geomevaluator->iso_evaluateGeometry(node, false);
	// if (geom) {
	// 	gt = evaluateCSGNodeFromGeometry(state, geom, node.modinst, node);
	// }
	// node.progress_report();
	// this->stored_leaf_term[node.index()] = dynamic_pointer_cast<CSGLeaf>(gt);
	// // if (this->geomevaluator) {
	// // 	std::cout << "geom evaluation..." << std::endl;
	// // 	auto geom = this->geomevaluator->evaluateGeometry(node, false);
	// // 	if (geom) {
	// // 		gt = evaluateCSGNodeFromGeometry(state, geom, node.modinst, node);
	// // 	}
	// // 	node.progress_report();
	// // }
	// // this->stored_leaf_term[node.index()] = dynamic_pointer_cast<CSGLeaf>(gt);
}

Response CSGTreeEvaluator::visit(State &state, const AbstractNode &node)
{
	// ichao : check the abstraction status
	// std::cout << "visit abstraction node" << std::endl;
	if (state.isPostfix()) {
		applyToChildren(state, node, OpenSCADOperator::UNION);
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

Response CSGTreeEvaluator::visit(State &state, const AbstractIntersectionNode &node)
{
	// ichao : check the abstraction status
	// std::cout << "visit abstract intersection node" << std::endl;
	if (state.isPostfix()) {
		applyToChildren(state, node, OpenSCADOperator::INTERSECTION);
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

shared_ptr<CSGNode> CSGTreeEvaluator::evaluateCSGNodeFromGeometry(
	State &state, const shared_ptr<const Geometry> &geom,
	const ModuleInstantiation *modinst, const AbstractNode &node)
{
	std::stringstream stream;
	stream << node.name() << node.index();

	// We cannot render Polygon2d directly, so we preprocess (tessellate) it here
	auto g = geom;
	if (!g->isEmpty()) {
		auto p2d = dynamic_pointer_cast<const Polygon2d>(geom);
		if (p2d) {
			// std::cout << "p2d tessellate" << std::endl;
			PolySet* p2d_poly = p2d->tessellate();
			p2d_poly->dim = 3;
			g.reset(p2d_poly);
			// g.reset(p2d->tessellate());
			std::cout << p2d_poly->getDimension() << std::endl;
		}
		else {
			// We cannot render concave polygons, so tessellate any 3D PolySets
			auto ps = dynamic_pointer_cast<const PolySet>(geom);
			// Since is_convex() doesn't handle non-planar faces, we need to tessellate
			// also in the indeterminate state so we cannot just use a boolean comparison. See #1061
			bool convex = ps->convexValue();
			if (ps && !convex) {
				assert(ps->getDimension() == 3);
				auto ps_tri = new PolySet(3, ps->convexValue());
				ps_tri->setConvexity(ps->getConvexity());
				PolysetUtils::tessellate_faces(*ps, *ps_tri);
				g.reset(ps_tri);
			}
		}
	}

	shared_ptr<CSGNode> t(new CSGLeaf(g, state.matrix(), state.color(), stream.str()));
	if (modinst->isHighlight()) t->setHighlight(true);
	else if (modinst->isBackground()) t->setBackground(true);
	return t;
}

Response CSGTreeEvaluator::visit(State &state, const AbstractPolyNode &node)
{
	// ichao : check the abstract poly status
	// std::cout << "visit abstract poly node" << std::endl;
	if (state.isPostfix()) {
		shared_ptr<CSGNode> t1;
		// std::cout << "visit abstract poly node postfix for poly node " << node.index() << std::endl;	
		if (this->geomevaluator) {
			// std::cout << "geom evaluation..." << std::endl;
			auto geom = this->geomevaluator->evaluateGeometry(node, false);
			if (geom) {
				t1 = evaluateCSGNodeFromGeometry(state, geom, node.modinst, node);
			}
			node.progress_report();
		}
		this->stored_term[node.index()] = t1;
		// ichao added
		// this->stored_leaf_term[node.index()] = dynamic_pointer_cast<CSGLeaf>(t1);	
		// std::cout << "stored term size : " << this->stored_term.size() << std::endl;
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

Response CSGTreeEvaluator::visit(State &state, const CsgOpNode &node)
{
	// ichao : check the csgopt status
	// std::cout << "visit csgopt node" << std::endl;
	if (state.isPostfix()) {
		applyToChildren(state, node, node.type);
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

Response CSGTreeEvaluator::visit(State &state, const TransformNode &node)
{
	// ichao : check the transformation status
	// std::cout << "visit transformation node" << std::endl;
	if (state.isPrefix()) {
		if (matrix_contains_infinity(node.matrix) || matrix_contains_nan(node.matrix)) {
			PRINT("WARNING: Transformation matrix contains Not-a-Number and/or Infinity - removing object.");
			return Response::PruneTraversal;
		}
		state.setMatrix(state.matrix() * node.matrix);
	}
	if (state.isPostfix()) {
		// std::cout << "transform number : " << node.index() << std::endl;
		applyToChildren(state, node, OpenSCADOperator::UNION);
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

Response CSGTreeEvaluator::visit(State &state, const ColorNode &node)
{
	if (state.isPrefix()) {
		if (!state.color().isValid()) state.setColor(node.color);
	}
	if (state.isPostfix()) {
		applyToChildren(state, node, OpenSCADOperator::UNION);
		addToParent(state, node);
	}	
	return Response::ContinueTraversal;
}

// FIXME: If we've got CGAL support, render this node as a CGAL union into a PolySet
Response CSGTreeEvaluator::visit(State &state, const RenderNode &node)
{
	if (state.isPostfix()) {
		shared_ptr<CSGNode> t1;
		shared_ptr<const Geometry> geom;
		if (this->geomevaluator) {
			geom = this->geomevaluator->evaluateGeometry(node, false);
			if (geom) {
				t1 = evaluateCSGNodeFromGeometry(state, geom, node.modinst, node);
			}
			node.progress_report();
		}
		this->stored_term[node.index()] = t1;
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

Response CSGTreeEvaluator::visit(State &state, const CgaladvNode &node)
{
	if (state.isPostfix()) {
		shared_ptr<CSGNode> t1;
    // FIXME: Calling evaluator directly since we're not a PolyNode. Generalize this.
		shared_ptr<const Geometry> geom;
		if (this->geomevaluator) {
			geom = this->geomevaluator->evaluateGeometry(node, false);
			if (geom) {
				t1 = evaluateCSGNodeFromGeometry(state, geom, node.modinst, node);
			}
			node.progress_report();
		}
		this->stored_term[node.index()] = t1;
		// this->stored_leaf_term[node.index()] = dynamic_pointer_cast<CSGLeaf>(t1);
		applyBackgroundAndHighlight(state, node);
		// this might be the reason why the number didnt match..
		// applyToChildren(state, node, OpenSCADOperator::UNION);
		addToParent(state, node);
	}
	return Response::ContinueTraversal;
}

/*!
	Adds ourself to out parent's list of traversed children.
	Call this for _every_ node which affects output during traversal.
    Usually, this should be called from the postfix stage, but for some nodes, we defer traversal letting other components (e.g. CGAL) render the subgraph, and we'll then call this from prefix and prune further traversal.
*/
void CSGTreeEvaluator::addToParent(const State &state, const AbstractNode &node)
{
	this->visitedchildren.erase(node.index());
	if (state.parent()) {
		this->visitedchildren[state.parent()->index()].push_back(&node);
	}
}
