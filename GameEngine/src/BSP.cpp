#include<iostream>
#include<memory>
#include<vector>
#include<cmath>
#include "Poly.h"
#include "BSP.h"
#include "Character.h"

using namespace std;


  BSPNode::BSPNode () { }
  BSPNode::~BSPNode () { }

  void BSPNode::setLeft(shared_ptr<BSPNode> left) {
    this->left = left;
  }
  void BSPNode::setRight(shared_ptr<BSPNode> right){
    this->right = right;
  }
  void BSPNode::setParent(BSPNode* parent){
    this->parent = parent;
  }
  void BSPNode::setPolygons(vector<Poly> polygons) {
    this->polygons = polygons;
  }
  void BSPNode::setSplit(TLine split) {
    this->split = split;
  }
  void BSPNode::addPolygon(Poly polygon) {
    this->polygons.push_back(polygon);
  }
  shared_ptr<BSPNode> BSPNode::getLeft() const {
    return this->left;
  }
  shared_ptr<BSPNode> BSPNode::getRight() const {
    return this->right;
  }
  vector<Poly> BSPNode::getPolygons() {
    return this->polygons;
  }
  BSPNode* BSPNode::getParent() const {
    return this->parent;
  }
  TLine BSPNode::getSplit() const {
    return this->split;
  }


  BSPTree::BSPTree () { }
  BSPTree::~BSPTree () { }

  void BSPTree::setRoot(shared_ptr<BSPNode> root) {
    this->root = root;
  }
  shared_ptr<BSPNode> BSPTree::getRoot() {
    return this->root;
  }

  TLine BSPTree::bestLine(vector<Poly> const &polygons) {
   // cout << "Finding the best line" << "\n";
    int diff = 0;
    TLine best;

    vector<Poly> right;
    vector<Poly> left;
    for (auto const &polygon : polygons) {
      //cout << "Current Polygon: " << "\n";
      //cout << polygon << "\n";
      for (auto const &line : polygon.getLines()) {
        for (auto const &p : polygons) {
          vector<Poly> v = Poly::subSections(p, line);
          int side = 0;
          if(v.size() == 0) {
            side = Poly::side(p, line);
            //cout << "Side: " << side << "\n";
            if (side == 1) {
              right.push_back(p);
            } else {
              left.push_back(p);
            }
          } else {
            int side1 = Poly::side(v[0], line);
            int side2 = Poly::side(v[1], line);
            //cout << "Side1: " << side1 << "\n";
            //cout << "Side2: " << side2 << "\n";
            if (side1 == 1) {
              right.push_back(v[0]);
            } else {
              left.push_back(v[0]);
            }
            if (side2 == 1) {
              right.push_back(v[1]);
            } else {
              left.push_back(v[1]);
            }
          }
        }
        int r = right.size();
        int l = left.size();
        //cout << "Right size: " << r << "\n";
        //cout << "Left size: " << l << "\n";
        //cout << "Sub: " << (r - l) << "\n";
        //cout << "Diff: " << abs(r - l) << "\n";
        if (diff == 0 || (diff > abs(r - l))) {
          diff = abs(r - l);
          best = line;
          //cout << "New Best Line: " << best << "\n";
        }
        right.clear();
        left.clear();
      }
    }
    //cout << "Best line found" << "\n";
    return best;
  }

  void BSPTree::generateTree(shared_ptr<BSPNode> root) {
    //cout << "Generating tree" << "\n";
    //cout << "Root size: " << root->getPolygons().size() << "\n";
    if(root->getPolygons().size() > 1) {
      shared_ptr<BSPNode> left(new BSPNode());
      shared_ptr<BSPNode> right(new BSPNode());
      TLine split = BSPTree::bestLine(root->getPolygons());
      //cout << "Best line: " << split << "\n";
      for (auto const &polygon : root->getPolygons()) {
        //cout << "Splitting polygon: " << "\n";
        //cout << polygon << "\n";
        vector<Poly> v = Poly::subSections(polygon, split);
        //cout << "Subsection count: " << v.size() << "\n";
        int side = 0;
        if(v.size() == 0) {
          side = Poly::side(polygon, split);

          if (side == 1) {
            right->addPolygon(polygon);
          } else {
            left->addPolygon(polygon);
          }
        } else {
          //cout << "Split part 1: " << v[0] << "\n";
          //cout << "Split part 2: " << v[1] << "\n";
          int side1 = Poly::side(v[0], split);
          int side2 = Poly::side(v[1], split);

          if (side1 == 1) {
            right->addPolygon(v[0]);
          } else {
            left->addPolygon(v[0]);
          }
          if (side2 == 1) {
            right->addPolygon(v[1]);
          } else {
            left->addPolygon(v[1]);
          }
        }
      }
      root->setSplit(split);
      root->setRight(right);
      root->setLeft(left);
      generateTree(root->getRight());
      generateTree(root->getLeft());
    }
  }

  void BSPTree::genLeafs(shared_ptr<BSPNode> root) {  
	  if (root->getLeft() == nullptr && root->getRight() == nullptr)
	  {
		  leafs.push_back(root);
	  }
	  if (root->getRight() != nullptr)
		  genLeafs(root->getRight());
	  if (root->getLeft() != nullptr)
		  genLeafs(root->getLeft());
  }

  vector<shared_ptr<BSPNode>> BSPTree::getLeafs() {
	  return leafs;
  }

    shared_ptr<BSPNode> BSPTree::findCharacter(shared_ptr<BSPNode> root, Character c) {
      if(root->getLeft() == NULL && root->getRight() == NULL) {
        return root;
      }
      double s = Vec::cross(c.getPosition() - root->getSplit().getStart(), root->getSplit().getMagnitude());
      if(s > 0) {
        return findCharacter(root->getRight(), c);
      }
      return findCharacter(root->getLeft(), c);
    }
