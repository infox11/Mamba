#ifndef Octree_H
#define Octree_H

#include <cstddef>
#include <vector>
#include "OctreePoint.h"


	class Octree {
		
		Vec3 origin;         
		Vec3 halfDimension;  
		Octree *children[8]; 
		OctreePoint *data;   

		public:
		Octree(const Vec3& origin, const Vec3& halfDimension) 
			: origin(origin), halfDimension(halfDimension), data(NULL) {
				for(int i=0; i<8; ++i) 
					children[i] = NULL;
			}

		Octree(const Octree& copy)
			: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {

			}

		~Octree() {
			for(int i=0; i<8; ++i) 
				delete children[i];
		}

		int getOctantContainingPoint(const Vec3& point) const {
			int oct = 0;
			if(point.x >= origin.x) oct |= 4;
			if(point.y >= origin.y) oct |= 2;
			if(point.z >= origin.z) oct |= 1;
			return oct;
		}

		bool isLeafNode() const {
			for (int i = 0; i<8; ++i)
				if (children[i] != NULL)
					return false;
			return true;
		}

		bool isLeafNodeTemp(Octree* node) const {
			for (int i = 0; i<8; ++i)
				if (node->children[i] != NULL)
					return false;
			return true;
		}

		int findRedudant() const {
			int counter = 0;
			int index = 0;
			for (int i = 0; i < 8; ++i) {
				if (!isLeafNodeTemp(children[i])) return -1;
				if (children[i]->data != NULL) {
					counter++;
					index = i;
				}
			}
			return (counter == 1) ? index : -1;
		}

		void remove(OctreePoint* point) {
			if (isLeafNode()) {
				if (data->getPosition() == point->getPosition()) {
					delete data;
					data = NULL;
				}
				return;
			}
			else {
				int octant = getOctantContainingPoint(point->getPosition());
				children[octant]->remove(point);
				
				int index = findRedudant();

				if (index != -1) {
					data = children[index]->data;
					for (int i = 0; i < 8; ++i) {
						delete children[i];
						children[i] = NULL;
					}
					//
					this->data = data;
				}
			}
		}

		void insert(OctreePoint* point) {
			if(isLeafNode()) {
				if(data==NULL) {
					data = point;
					return;
				} else {
					
					OctreePoint *oldPoint = data;
					data = NULL;
					for(int i=0; i<8; ++i) {
						Vec3 newOrigin = origin;
						newOrigin.x += halfDimension.x * (i&4 ? .5f : -.5f);
						newOrigin.y += halfDimension.y * (i&2 ? .5f : -.5f);
						newOrigin.z += halfDimension.z * (i&1 ? .5f : -.5f);
						children[i] = new Octree(newOrigin, halfDimension*.5f);
					}
					children[getOctantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
					children[getOctantContainingPoint(point->getPosition())]->insert(point);
				}
			} else {
				int octant = getOctantContainingPoint(point->getPosition());
				children[octant]->insert(point);
			}
		}

		
		void getPointsInsideBox(const Vec3& bmin, const Vec3& bmax, std::vector<OctreePoint*>& results) {

			if(isLeafNode()) {
				if(data!=NULL) {
					const Vec3& p = data->getPosition();
					if(p.x>bmax.x || p.y>bmax.y || p.z>bmax.z) return;
					if(p.x<bmin.x || p.y<bmin.y || p.z<bmin.z) return;
					results.push_back(data);
				}
			} else {

				for(int i=0; i<8; ++i) {

					Vec3 cmax = children[i]->origin + children[i]->halfDimension;
					Vec3 cmin = children[i]->origin - children[i]->halfDimension;

					if(cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
					if(cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

					children[i]->getPointsInsideBox(bmin,bmax,results);
				} 
			}
		}
		
		void deletePointsInsideBox(const Vec3& bmin, const Vec3& bmax) {

			if (isLeafNode()) {
				if (data != NULL) {
					const Vec3& p = data->getPosition();
					if (p.x > bmax.x || p.y > bmax.y || p.z > bmax.z) return;
					if (p.x < bmin.x || p.y < bmin.y || p.z < bmin.z) return;
					remove(new OctreePoint(p));
				}
			}
			else {

				for (int i = 0; i<8; ++i) {

					Vec3 cmax = children[i]->origin + children[i]->halfDimension;
					Vec3 cmin = children[i]->origin - children[i]->halfDimension;

					if (cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
					if (cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

					children[i]->deletePointsInsideBox(bmin, bmax);
				}
			}
		}
			

			
		
	};


#endif
