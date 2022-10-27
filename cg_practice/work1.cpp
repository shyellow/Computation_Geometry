#include<iostream>
using namespace std;

struct Point
{
	double x;
	double y;
	bool extreme;
};
//ccw(判断p3在直线p1p2的哪一侧,左侧为正)
int ccw(Point p1, Point p2, Point p3) {
	return (p1.x * p2.y + p2.x * p3.y + p3.x * p1.y) - (p1.x * p3.y + p2.x * p1.y + p3.x * p2.y) > 0 ? 1 : -1;
}

//begin(1)----Exreme Point
//return Area determinant
int Area2(Point p, Point q, Point s) {
	return p.x * q.y - p.y * q.x
		+ q.x * s.y - q.y * s.x
		+ s.x * p.y - s.y * p.x;
}
bool ToLeft(Point p, Point q, Point s) {
	return Area2(p, q, s) > 0;//determinant is +（on the left）
}
//in triangle ?
bool InTriangle(Point p, Point q, Point r, Point s) {
	bool pqLeft = ToLeft(p, q, s);
	bool qrLeft = ToLeft(q, r, s);
	bool rqLeft = ToLeft(r, q, s);
	return (pqLeft == qrLeft) && (qrLeft == rqLeft);//all is true/false
}
//ExtremePoint Algorithm
void extremePoint(Point S[], int n) {//n > 2
	for (int s = 0; s < n; s++) S[s].extreme = true;
	for(int p = 0; p < n; p++)
		for(int q = p+1; q < n; q++)
			for(int r = q+1; r < n; r++)
				for (int s = 0; s < n; s++) {
					if (s == p || s == q || s == r || !S[s].extreme)//is extreme
						continue;
					if (InTriangle(S[p], S[q], S[r], S[s]))//in Triangle
						S[s].extreme = false;
				}
}
//end(1)----Exreme Point

//begin(2)----Exreme Edge
void checkEdge(Point S[], int n, int p, int q) {
	bool LEmpty = true, REmpty = true;
	for (int k = 0; k < n && (LEmpty || REmpty); k++)
		if (k != p && k != q)
			ToLeft(S[p], S[q], S[k]) ? LEmpty = false : REmpty = false;
	if (LEmpty || REmpty)
		S[p].extreme = S[q].extreme = true;
		
}
void markEE(Point S[], int n) {//n > 2
	for (int k = 0; k < n; k++) {
		S[k].extreme = false;
	}
	for (int p = 0; p < n; p++)
		for (int q = p + 1; q < n; q++)
			checkEdge(S, n, p, q);
}
//end(2)----Extreme Edge

//begin(3)----Incremental Construction
void incremental(Point n[]) {
	if (sizeof(n) / sizeof(n[0]) < 3) return;
	Point t = n[0];
}
//end(3)

int main() {
	
	return 0;
}