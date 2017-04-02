#include <queue>
#include <iostream>

#include "astar.h"

using namespace sf;

Astar::Node Astar::graph[G_WIDTH][G_HEIGHT] = {};
Vector2i Astar::from = Vector2i(0, 0);
Vector2i Astar::to = Vector2i(0, 0);

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

std::vector<Vector2i> Astar::findPath() {
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

	std::priority_queue<Node*, std::vector<Node*>, Node::Comparator> frontier;
	frontier.push(start);

	while (!frontier.empty()) {
		auto current = frontier.top();
		frontier.pop();

		if (current->getVal() == to) {
			goal = current;
			break;
		}

		std::vector<Node*> nb = current->getNeighbors();
		for (auto n : nb) {
			if (!n->isVisited()) {
				n->setCost(n->getEstimatedCost(), current->getCostSoFar() + n->getWeight());
				n->setVisited(true);
				n->setParent(current);
				frontier.push(n);
			}
		}
	}

	std::vector<Vector2i> res(0);
	while (goal != NULL && goal->getParent() != NULL) {
		res.push_back(goal->getVal());
		goal = goal->getParent();
	}

	return res;
}

void Astar::hardReset(std::vector<Vector2i>& obstacles) {
	for (int i = 0; i < G_WIDTH; i++) {
		for (int j = 0; j < G_HEIGHT; j++) {
			graph[i][j].setVisited(false);
		}
	}

	std::queue<Node*> q;
	for (auto v : obstacles) {
		graph[v.x][v.y].setParent(NULL);
		graph[v.x][v.y].setCost(manhattan(Vector2i(v.x, v.y), to), 0);
		graph[v.x][v.y].setWeight(WEIGHT_REF);
		graph[v.x][v.y].setVisited(true);
		q.push(&graph[v.x][v.y]);
	}

	while (!q.empty()) {
		auto current = q.front();
		q.pop();

		std::vector<Node*> nb = current->getNeighbors();
		for (auto n : nb) {
			if (!n->isVisited()) {
				n->setParent(NULL);
				n->setCost(manhattan(Vector2i(n->getVal().x, n->getVal().y), to), 0);
				n->setWeight(current->getWeight() - 1);
				n->setVisited(true);
				q.push(n);
			}
		}
	}
}

void Astar::softReset(std::vector<Vector2i> obstacles) {
	for (int i = 0; i < G_WIDTH; i++) {
		for (int j = 0; j < G_HEIGHT; j++) {
			graph[i][j].setVisited(false);
			graph[i][j].setParent(NULL);
			graph[i][j].setCost(manhattan(Vector2i(i, j), to), 0);
			graph[i][j].setWeight(0);
		}
	}

	for (auto v : obstacles) {
		graph[v.x][v.y].setParent(NULL);
		graph[v.x][v.y].setCost(manhattan(Vector2i(v.x, v.y), to), 0);
		graph[v.x][v.y].setWeight(WEIGHT_REF);
	}
}


void Astar::setTo(Vector2i t) {
	to = t;
}

void Astar::setFrom(Vector2i f) {
	from = f;
}

float Astar::manhattan(const sf::Vector2i& v1, const sf::Vector2i& v2) {
	return (float)(abs(v2.x - v1.x) + abs(v2.y - v1.y));
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

//Comparator
bool Astar::Node::Comparator::operator()(const Node* a, const Node* b) {
	return (a->getEstimatedCost()+a->getCostSoFar()) > (b->getEstimatedCost()+b->getCostSoFar());
}