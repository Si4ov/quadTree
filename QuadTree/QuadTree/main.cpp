
#include <cmath>
#include <iostream>
using namespace std;

// Used to hold details of a point
struct Point {
	int x;
	int y;
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point()
	{
		x = 0;
		y = 0;
	}
};

// The objects that we want stored in the quadtree
struct Node {
	Point pos;
	int data;
	Node(Point _pos, int _data)
	{
		pos = _pos;
		data = _data;
	}
	Node() { data = 0; }
};

// The main quadtree class
class Quad {
	//  Boundary of this node
	Point botLeft;
	Point topRight;

	// details of node
	Node* n;

	// quads of the tree
	Quad* topLeftTree;
	Quad* topRightTree;
	Quad* botLeftTree;
	Quad* botRightTree;

public:
	Quad()
	{
		botLeft = Point(0, 0);
		topRight = Point(0, 0);
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
	}
	Quad(Point botL, Point topR)
	{
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		botLeft = botL;
		topRight = topR;
	}
	void insert(Node*);
	Node* search(Point);
	bool inBoundary(Point);
};

// Check if current quadtree contains the point
bool Quad::inBoundary(Point p)
{
	return (p.x >= botLeft.x && p.x <= topRight.x
		&& p.y >= botLeft.y && p.y <= topRight.y);
}

// Insert a node into the quadtree
void Quad::insert(Node* node)
{
	if (node == NULL)
		return;

	// Current quad cannot contain it
	if (!inBoundary(node->pos))
		return;

	// We are at a quad of unit area
	// We cannot subdivide this quad further
	if (abs(botLeft.x - topRight.x) <= 1
		&& abs(botLeft.y - topRight.y) <= 1) {
		if (n == NULL)
			n = node;
		return;
	}

	if ((botLeft.x + topRight.x) / 2 >= node->pos.x) {
		// Indicates BotLeftTree
		if ((botLeft.y + topRight.y) / 2 >= node->pos.y) {
			if (botLeftTree == NULL)
				botLeftTree = new Quad(
					Point(botLeft.x, botLeft.y),
					Point((botLeft.x + topRight.x) / 2,
					(botLeft.y + topRight.y) / 2));
			botLeftTree->insert(node);
		}

		// Indicates TopLeftTree
		else {
			if (topLeftTree == NULL)
				topLeftTree = new Quad(
					Point(botLeft.x,
					(botLeft.y + topRight.y) / 2),
					Point((botLeft.x + topRight.x) / 2,
						topRight.y));
			topLeftTree->insert(node);
		}
	}
	else {
		// Indicates BotRightTree
		if ((botLeft.y + topRight.y) / 2 >= node->pos.y) {
			if (botRightTree == NULL)
				botRightTree = new Quad(
					Point((botLeft.x + topRight.x) / 2,
						botLeft.y),
					Point(topRight.x,
					(botLeft.y + topRight.y) / 2));
			botRightTree->insert(node);
		}

		// Indicates TopRightTree
		else {
			if (topRightTree == NULL)
				topRightTree = new Quad(
					Point((botLeft.x + topRight.x) / 2,
					(botLeft.y + topRight.y) / 2),
					Point(topRight.x, topRight.y));
			topRightTree->insert(node);
		}
	}
}

// Find a node in a quadtree
Node* Quad::search(Point p)
{
	// Current quad cannot contain it
	if (!inBoundary(p))
		return NULL;

	// We are at a quad of unit length
	// We cannot subdivide this quad further
	if (n != NULL)
		return n;

	if ((botLeft.x + topRight.x) / 2 >= p.x) {
		// Indicates topLeftTree
		if ((botLeft.y + topRight.y) / 2 >= p.y) {
			if (botLeftTree == NULL)
				return NULL;
			return botLeftTree->search(p);
		}

		// Indicates botLeftTree
		else {
			if (topLeftTree == NULL)
				return NULL;
			return topLeftTree->search(p);
		}
	}
	else {
		// Indicates topRightTree
		if ((botLeft.y + topRight.y) / 2 >= p.y) {
			if (botRightTree == NULL)
				return NULL;
			return botRightTree->search(p);
		}

		// Indicates botRightTree
		else {
			if (topRightTree == NULL)
				return NULL;
			return topRightTree->search(p);
		}
	}
};



int main()
{
	Quad center(Point(0, 0), Point(8, 8));
	Node a(Point(1, 1), 1);
	Node b(Point(2, 5), 2);
	Node c(Point(7, 6), 3);
	Node d(Point(7, 7), 15);
	center.insert(&a);
	center.insert(&b);
	center.insert(&c);
	center.insert(&d);
	cout << "Node a: " << center.search(Point(1, 1))->data
		<< "\n";
	cout << "Node b: " << center.search(Point(2, 5))->data
		<< "\n";
	cout << "Node c: " << center.search(Point(7, 6))->data
		<< "\n";
	cout << "Node d: " << center.search(Point(7, 7))->data
		<< "\n";
	cout << "Non-existing node: "
		<< center.search(Point(1, 2)) <<"\n";


	std::cout << "Press Enter to exit...";
	std::cin.get(); 

	return 0;
}

