#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "glibby/primitives/point.h"

#include <memory>
#include <vector>

namespace glibby 
{
  class QuadTreeNode 
  {
    public:
      /**
       * @brief Constructor for QuadTreeNode.
       *
       * Each QuadTreeNode has a boundary defining an area from which points
       * can be inserted. Points lying outside that area are not valid, and
       * should not be added to the node.
       *
       * Additionally, each node has a capacity for number of nodes which 
       * should not be passed in order to retain an even distribution of points
       * throughout the QuadTree.
       *
       * Each node has four sub-nodes (NorthEast, NorthWest, SouthEast,
       * SouthWest). Each sub-node's area is contained within the parent's area.
       *
       * @param cen - shared pointer to Point2D, center of the area
       * @param width - overall width of area
       * @param height - overall height of area
       * @param cap - capacity of the QuadTreeNode
       * */
      QuadTreeNode(std::shared_ptr<Point2> cen, float width, float height, 
          int cap);

      /**
       * @brief Check if Point2D is inside the boundary definded by the
       * QuadTreeNode
       *
       * @param ptr - pointer to Point2D
       *
       * @return true if inside boundary, false otherwise
       * */
      bool inside_boundary(Point2* ptr) const;
      /**
       * @brief Check if boundary defined by the parameters will intersect
       * with the boundary defined by the QuadTreeNode.
       *
       * @param center - pointer to Point2D, center of area contained by 
       * boundary
       * @param width - overall width of the boundary, the left edge is center.x
       * - width / 2, the right edge is center.x + width / 2
       * @param height - overall height of the boundary, the top edge is 
       * center.y + height / 2, the bottom edge is center.y - height / 2
       *
       * @return true if boundaries intersect, false otherwise
       * */
      bool intersect_boundary(Point2* center, float width, float height) const;

      std::shared_ptr<Point2> center_;
      long unsigned capacity_;
      float width_;
      float height_;
      bool divided_;
      std::shared_ptr<QuadTreeNode> NW_;
      std::shared_ptr<QuadTreeNode> NE_;
      std::shared_ptr<QuadTreeNode> SW_;
      std::shared_ptr<QuadTreeNode> SE_;
      std::vector<std::shared_ptr<Point2>> points_;
  };

  class QuadTree 
  {
    public:
      /**
       * @brief Will create QuadTree with boundary defined by parameters. The
       * boundary is rectangular.
       *
       * @param TopL - point denoting top left of boundary
       * @param TopR - point denoting top right of boundary
       * @param BotL - point denoting bottom left of boundary
       * @param BotR - point denoting bottom right of boundary
       * @param capacity - capacity of each node in tree, lower capacity means
       * higher depth, default is 1
       * */
      QuadTree(std::shared_ptr<Point2> TopL, std::shared_ptr<Point2> TopR, 
          std::shared_ptr<Point2> BotL, std::shared_ptr<Point2> BotR, 
          int capacity = 1);
      /**
       * @brief Will create QuadTree with boundary defined by parameters. The
       * boundary is rectangular.
       *
       * @param p - point the center of the boundary
       * @param width - overall width of boundary
       * @param height - overall height of boundary
       * @param capacity - capacity of each node in tree, lower capacity means
       * higher depth, default is 1
       * */
      QuadTree(std::shared_ptr<Point2> p, float width, float height, 
          int capacity = 1);

      /**
       * @brief Will insert point into QuadTree at correct subnode if the point
       * is valid. Valid nodes are those within the boundary defined by the
       * information passed to the constructor.
       *
       * @param point - point to be inserted
       *
       * @return true if point was successfully added, false otherwise, if 
       * false, it is likely that point is not in boundary of QuadTree
       * */
      bool insert(Point2* point);
      /**
       * @brief Will remove point from QuadTree if the point is in the tree.
       * If multiple copies of the same point are in the tree, only the first
       * point found will be removed.
       *
       * @param point - point to be removed
       *
       * @return true if point was successfully removed, false otherwise, if 
       * false, it is likely that point is not in the QuadTree
       * */
      bool remove(Point2* point);
      /**
       * @brief Check if point has been added to the QuadTree
       *
       * @param point - point to check if inserted
       *
       * @return true if point is in tree, false otherwise
       * */
      bool contains(Point2* point) const;
      /**
       * @brief Find all points within a boundary defined by parameters
       *
       * @param point - center of boundary
       * @param width - overall width of boundary
       * @param height - overall height of boundary
       *
       * @return A vector of Point2D. Each Point2D is a copy of a point within
       * the tree
       * */
      std::vector<Point2> query(Point2* point, float width, 
          float height) const;

      int size() const {return size_;};
      /**
       * @brief removes all points from tree
       * */
      void clear();

    private:
      /*
       * recursively finds/creates correct node to insert point and inserts
       * the point there, starts at given node
       * */
      bool add_point(std::shared_ptr<QuadTreeNode> node, Point2* point);
      /*
       * recursively finds point and removes it
       * */
      bool remove_point(std::shared_ptr<QuadTreeNode> node, Point2* point);
      /*
       * will subdivide a node, creating the proper children
       * */
      void subdivide(std::shared_ptr<QuadTreeNode> node);
      /*
       * recursively searches tree for a specific point. starts at given
       * node
       * */
      bool search(std::shared_ptr<QuadTreeNode> node, Point2* point) const;
      /*
       * recursively finds all points within given boundary and adds copies of
       * those points to the given vector
       * */
      void search_tree(
          std::vector<Point2>* points, std::shared_ptr<QuadTreeNode> node, 
          Point2* center, float width, float height) const;
      
      std::shared_ptr<QuadTreeNode> node_;
      int capacity_;
      int size_;
  };
}

#endif
