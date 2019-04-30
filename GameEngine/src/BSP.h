#ifndef BSPT_H
#define BSPT_H

#include<iostream>
#include<memory>
#include<vector>
#include "Poly.h"
#include "Line.h"
#include "Character.h"

using namespace std;

class BSPNode {

private:
  vector<Poly> polygons;
  shared_ptr<BSPNode> left;
  shared_ptr<BSPNode> right;
  BSPNode *parent;
  TLine split;

public:
  BSPNode ();
  virtual ~BSPNode ();

  void setLeft(shared_ptr<BSPNode> left);
  void setRight(shared_ptr<BSPNode> right);
  void setParent(BSPNode* parent);
  void setPolygons(vector<Poly> polygons);
  void setSplit(TLine split);
  void addPolygon(Poly polygon);
  shared_ptr<BSPNode> getLeft() const;
  shared_ptr<BSPNode> getRight() const;
  vector<Poly> getPolygons();
  BSPNode* getParent() const;
  TLine getSplit() const;
};

class BSPTree {

private:
  shared_ptr<BSPNode> root;

  vector<shared_ptr<BSPNode>> leafs;


public:
  BSPTree ();
  virtual ~BSPTree ();


  void setRoot(shared_ptr<BSPNode> root);

  shared_ptr<BSPNode> getRoot();

  TLine static bestLine(vector<Poly> const &polygons);
  void static generateTree(shared_ptr<BSPNode> root);
  shared_ptr<BSPNode> findCharacter(shared_ptr<BSPNode> root, Character c);
  void genLeafs(shared_ptr<BSPNode> root);
  vector<shared_ptr<BSPNode>> getLeafs();
};


#endif
