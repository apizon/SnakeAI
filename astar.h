#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <vector>
#include <SFML/Graphics.hpp>

#include "define.h"

class Astar {
	private:
		class Node {
			public:
				Node();
				Node(sf::Vector2i& v, const std::vector<Node*>& nb, Node* p = NULL, float ec = 0, float w = 0, bool vis = false);
				void setParent(Node* p);
				void setCost(float ec, float csf);
				void setWeight(float w);
				void setVisited(bool v);
				Node* getParent() const;
				std::vector<Node*> getNeighbors() const;
				sf::Vector2i getVal() const;
				float getEstimatedCost() const;
				float getCostSoFar() const;
				float getWeight() const;
				bool isVisited() const;

				class Comparator {
					public:
						bool operator()(const Node* a, const Node* b);
				};

			private:
				Node* parent;
				std::vector<Node*> neighbors;
				sf::Vector2i val;
				float estimated_cost, cost_so_far;
				float weight;
				bool visited;
		};

		static sf::Vector2i from;
		static sf::Vector2i to;
		static Node graph[G_WIDTH][G_HEIGHT];

	public:
		static void init();
		static std::vector<sf::Vector2i> findPath();
		static void hardReset(std::vector<sf::Vector2i>& obstacles);
		static void softReset(std::vector<sf::Vector2i>& obstacles);
		static void update(sf::Vector2i v);
		static void setTo(sf::Vector2i t);
		static void setFrom(sf::Vector2i f);
		static float manhattan(const sf::Vector2i& v1, const sf::Vector2i& v2);
		static float getWeight(int i, int j);
		static float getCost(int i, int j);
};

#endif

