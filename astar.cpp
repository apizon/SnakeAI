#include <queue>
#include <iostream>

#include "astar.h"

using namespace sf;

// Static members of Astar i had to redeclare here
Vector2i Astar::from;
Vector2i Astar::to;
Astar::Node Astar::graph[G_WIDTH][G_HEIGHT];

// Set each node's neighbors, this is the only time we do this as they never change
void Astar::init() {
	for (int i = 0; i < G_WIDTH; i++) {
		for (int j = 0; j < G_HEIGHT; j++) {
			std::vector<Node*> neighbors(0);
			if (i > 0)
				neighbors.push_back(&graph[i - 1][j]);
			if (j > 0)
				neighbors.push_back(&graph[i][j - 1]);
			if (i < G_WIDTH - 1)
				neighbors.push_back(&graph[i + 1][j]);
			if (j < G_HEIGHT - 1)
				neighbors.push_back(&graph[i][j + 1]);

			graph[i][j] = Node(Vector2i(i, j), neighbors);
		}
	}
}

//Main function
std::vector<Vector2i> Astar::findPath(Config& config) {
	// Set all obstacles Node as already visited and all others as not visited
	for (int i = 0; i < G_WIDTH; i++) {
		for (int j = 0; j < G_HEIGHT; j++) {
			if (graph[i][j].getWeight() == WEIGHT_REF)
				graph[i][j].setVisited(true);
			else
				graph[i][j].setVisited(false);
		}
	}

	Node* start = &graph[from.x][from.y];
	Node* goal = NULL;

	// Priority queue using pointers to the grid's nodes
	std::priority_queue<Node*, std::vector<Node*>, Node::Comparator> frontier;
	// We start by adding the departure point
	frontier.push(start);

	// While there are somme unvisited nodes
	while (!frontier.empty()) {
		// Recover first element...
		auto current = frontier.top();
		// ...and remove it from queue
		frontier.pop();

		// If we've reached destination we exit the loop
		if (current->getVal() == to) {
			goal = current;
			break;
		}

		// Recover the node's neighbors
		std::vector<Node*> nb = current->getNeighbors();
		for (auto n : nb) {
			// If we never went to that node and he is not an obstacles
			if (!n->isVisited()) {
				// If the cost diplay is on we have already calculated all the manhattan values so we just recover it
				// If it's off we just calculate it live, which is faster but the display is nice for presentation 
				// purposes as it shows a nice heatmap around the apple to really see what's going on

				// I'll try to explain how the estimated cost influence the AI as i didn't seem to get it right during 
				// the oral presentation

				// Each node have a value between 0 and WEIGHT_REF (=G_WIDTH+G_HEIGHT)
				// What we do is bring that value between 1 and 2, so that nodes closer to the snake have a value
				// that is closer to 2 and the farthers have a value closer to 1
				// When we pass through a node we add that value to the cost_so_far, which is used by the comparator
				// when adding a node to the queue
				// What it is supposed to do is find a path that's farther away from the snakes body and the other snakes
				// This makes the path sometimes longer and harder to find but it's supposedly a safer path
				// It's hard to really say if it's effective as the game value a lot depending on randomness, but
				// the snake seems to reach higher scores when this process is on, although it is slower
				
				// The process is enabled when we used hardReset() and disabled when using softReset()
				// It can be toggled while running using Numpad0, you should see a slight change in the way the snake move
				// and a pretty good change on his speed
				if(config.isDisplayCostOn())
					n->setCost(n->getEstimatedCost(), current->getCostSoFar() + 1 + n->getWeight() / WEIGHT_REF);
				else
					n->setCost(manhattan(Vector2i(n->getVal().x, n->getVal().y), to), current->getCostSoFar() + 1 + n->getWeight() / WEIGHT_REF);
				// We set it as visited, set where we came from and add it to the queue
				n->setVisited(true);
				n->setParent(current);
				frontier.push(n);
			}
		}
	}

	std::vector<Vector2i> res;
	// We just go back in the parents node to figure out the path until we reach the last one
	while (goal != NULL && goal->getParent() != NULL) {
		res.push_back(goal->getVal());
		goal = goal->getParent();
	}

	return res;
}

// Reset nodes by giving them a weight value depending on their distance to the snake
// We use a simple BFS algorithm
// It add complexity and slow the game but usually makes for better scores as it makes
// the snake take longer paths to dodge his body and the other snakes
void Astar::hardReset(std::vector<Vector2i>& obstacles) {
	// Set all nodes as not visited
	for (int i = 0; i < G_WIDTH; i++) {
		for (int j = 0; j < G_HEIGHT; j++) {
			graph[i][j].setVisited(false);
		}
	}

	// Put all the nodes containing a snake part into the queue, set them as visited and
	// put their weight to the max value
	std::queue<Node*> q;
	for (auto v : obstacles) {
		graph[v.x][v.y].setParent(NULL);
		graph[v.x][v.y].setWeight(WEIGHT_REF);
		graph[v.x][v.y].setVisited(true);
		q.push(&graph[v.x][v.y]);
	}

	// We just get the first element of the queue, get it's neighbors, give them the weight of
	// their parent - 1, set them as visited and put them in the queue
	// Rinse and repeat until all the map is visited
	while (!q.empty()) {
		auto current = q.front();
		q.pop();

		std::vector<Node*> nb = current->getNeighbors();
		for (auto n : nb) {
			if (!n->isVisited()) {
				n->setParent(NULL);
				n->setWeight(current->getWeight() - 1);
				n->setVisited(true);
				q.push(n);
			}
		}
	}
}

// Reset nodes by setting their weight to 0 if they're empty or max (WEIGHT_REF) if they contain
// a snake's body part
// Faster but usually make the snake die sooner as it doesn't try to dodge his body or other snakes
void Astar::softReset(std::vector<Vector2i>& obstacles) {
	for (int i = 0; i < G_WIDTH; i++) {
		for (int j = 0; j < G_HEIGHT; j++) {
			graph[i][j].setVisited(false);
			graph[i][j].setParent(NULL);
			graph[i][j].setWeight(0);
		}
	}

	for (auto v : obstacles) {
		graph[v.x][v.y].setParent(NULL);
		graph[v.x][v.y].setWeight(WEIGHT_REF);
	}
}

// Used to add a snake's head new position to the obstacles list, as we only reset the grid once
// per frame and not once per snake
void Astar::update(Vector2i v) {
	graph[v.x][v.y].setWeight(WEIGHT_REF);
}


// If the cost is displayed, we calculate the distance of each node to the apple
// to make for a nice gradient
// Slower and only to be used for presentation/visual purposes
void Astar::setTo(Vector2i t, Config& config) {
	to = t;
	if (config.isDisplayCostOn()) {
		for (int i = 0; i < G_WIDTH; i++) {
			for (int j = 0; j < G_HEIGHT; j++) {
				graph[i][j].setCost(manhattan(Vector2i(i, j), to), 0);
			}
		}
	}
}

void Astar::setFrom(Vector2i f) {
	from = f;
}

float Astar::manhattan(const sf::Vector2i& v1, const sf::Vector2i& v2) {
	return (float)(abs(v2.x - v1.x) + abs(v2.y - v1.y));
}

float Astar::getWeight(int i, int j) {
	return graph[i][j].getWeight();
}

float Astar::getCost(int i, int j) {
	return graph[i][j].getEstimatedCost();
}

//Node
Astar::Node::Node() {
	parent = NULL;
	val = Vector2i(0, 0);
	estimated_cost = 0;
	cost_so_far = 0;
	weight = 0;
	visited = false;
}

Astar::Node::Node(Vector2i& v, const std::vector<Node*>& nb, Node* p, float ec, float w, bool vis) {
	val = v;
	neighbors = nb;
	parent = p;
	estimated_cost = ec;
	cost_so_far = 0;
	weight = w;
	visited = vis;
}

void Astar::Node::setParent(Node* p) {
	parent = p;
}

void Astar::Node::setCost(float ec, float csf) {
	estimated_cost = ec;
	cost_so_far = csf;
}

void Astar::Node::setWeight(float w) {
	weight = w;
}

void Astar::Node::setVisited(bool v) {
	visited = v;
}

Astar::Node* Astar::Node::getParent() const{
	return parent;
}

std::vector<Astar::Node*> Astar::Node::getNeighbors() const {
	return neighbors;
}

Vector2i Astar::Node::getVal() const {
	return val;
}

float Astar::Node::getEstimatedCost() const {
	return estimated_cost;
}

float Astar::Node::getCostSoFar() const {
	return cost_so_far;
}

float Astar::Node::getWeight() const{
	return weight;
}

bool Astar::Node::isVisited() const {
	return visited;
}

// Comparator
// It basically adds the distance to the destination to the cost it took to get where we are
// It might be closer in this way to Dijkstra's algorithm than to A* and maybe i confused both
// I looked to redblobgames.com to get the general principle when i was making the game and the
// the way i understood it, A* is a mix between Greedy BFS and dijkstra
// Source : http://www.redblobgames.com/pathfinding/a-star/introduction.html#astar
// I suppose this confusion with the names was part of what made my oral presentation a bit messy
// and/or hard to understand
bool Astar::Node::Comparator::operator()(const Node* a, const Node* b) {
	return (a->getEstimatedCost()+a->getCostSoFar()) > (b->getEstimatedCost()+b->getCostSoFar());
}