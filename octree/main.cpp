#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Octree.h"

std::vector<Vec3> points;
Octree *octree;
OctreePoint *octreePoints;


using namespace std;



int main(int argc, char **argv) {
	setlocale(LC_ALL, "Russian");
	
	char v;
	double x = 0;
	double y = 0;
	double z = 0;
	double a = 0;
	double b = 0;
	double c = 0;
	

	double h = 0;
	cout << "¬веди halfdim (половина стороны куба)" << endl;
	cin >> h;

	const Vec3 origin = Vec3(0, 0, 0);
	const Vec3 halfdim = Vec3(h, h, h);
	const Vec3 min = -1 * halfdim;
	const Vec3 max = halfdim;
	octree = new Octree(origin, halfdim); // origin - это центр куба(начало), а halfdim - это половина стороны куба
	
	
	enum Commands {Insert = 'i', ShowPoint = 's', Remove = 'r',Exit ='e' };
	while (true) {
		cout << "e=Exit, i=Insert, s=ShowPoint, r=Remove" << endl;
		cin >> v;
		std::vector<OctreePoint*> results;
		
		switch (v) {
			
		case Insert:
			cout << "¬веди координаты точки" << endl;
			cin >> x >> y >> z;
			
			//octree->insert(new OctreePoint(*(new Vec3(x, y, z))));
			octree->insert(new OctreePoint(Vec3(x, y, z)));
			break;
			cout << "Inserted" << endl;

		case Remove: // по диагонали строитс€ параллелограм (коробка) и удал€ютс€ все точки внутри этой коробки 
			cout << "¬веди координаты точек диагонали(удал€тс€ все точки внутри коробки )" << endl;
			cout << "ѕерва€ точка (min):" << endl;
			cin >> x >> y >> z;
			cout << "¬тора€ точка (max):" << endl;
			cin >> a >> b >> c;
			octree->deletePointsInsideBox( Vec3 (x, y, z), Vec3 (a, b, c));
			cout << "Removed" << endl;
            break;
		
		case ShowPoint:// показывает точки только внутри заданного куба (c координатами <= halfdim)

			cout << "ShowPoint (¬нутри куба)"<< endl;
			octree->getPointsInsideBox(min, max, results);

			for (int i = 0; i < results.size(); i++) {
				cout << *results[i] << endl;
			}
			break;

		case Exit:
			exit(EXIT_SUCCESS);
			
		default:
			cout << "Wrong!" << endl;
			break;
		}
	}
	
	/*int main(int argc, char **argv) {
	octree = new Octree(Vec3(0, 0, 0),Vec3 (1, 1, 1));
	const Vec3 max(1, 1, 1);
	const Vec3 min(-1, -1, -1);
	std::vector<OctreePoint*> results;

	/*const Vec3 a1(0.5, 0.5, 0.5);
	const Vec3 a2(0.75, 0.75, 0.75);
	const Vec3 a3(0.76, 0.76, 0.76);
	const Vec3 a4(0.55, 0.54, 0.51);
	const Vec3 a5(1, 1, 1);
	const Vec3 a6(0.756 , 0.756, 0.756);
	

	octree->insert(new OctreePoint(a1));
	octree->insert(new OctreePoint(a2));
	octree->insert(new OctreePoint(a3));
	octree->insert(new OctreePoint(a4));
	octree->insert(new OctreePoint(a5));
	octree->insert(new OctreePoint(a6));
	*/
	
	
	
	/*double x = 0.5;
		double y = 0.5;
		double z = 0.5;

	octree->insert(new OctreePoint(Vec3(x, y, z)));
	octree->getPointsInsideBox(min, max, results);

	for (int i = 0; i < results.size(); i++)
		cout << *results[i] << endl;

	*/
	
	
		//cin.get();
		return 0;
	}
