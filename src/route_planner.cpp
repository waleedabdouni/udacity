#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
	start_node = &(m_Model.FindClosestNode (start_x, start_y));
    end_node = &(m_Model.FindClosestNode (end_x, end_y));
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
	
  return (*node).distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
	current_node->FindNeighbors();
  	for(std::vector<RouteModel::Node *>::size_type i = 0; i < current_node->neighbors.size(); i++) {
      	current_node->neighbors[i]->parent = current_node;
        current_node->neighbors[i]->h_value = CalculateHValue(current_node->neighbors[i]);
      	current_node->neighbors[i]->g_value = current_node->neighbors[i]->g_value + current_node->neighbors[i]->distance(*current_node);
      	open_list.push_back(current_node->neighbors[i]);
      	current_node->neighbors[i]->visited = true;
	}
}

bool myCompFunction (RouteModel::Node* nodeOne, RouteModel::Node* nodeTwo) {
  float sumOne = nodeOne->h_value + nodeOne->g_value;
  float sumTwo = nodeTwo->h_value + nodeTwo->g_value;
  return (sumOne > sumTwo); 
}

// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
  	std::sort (open_list.begin(), open_list.end(), myCompFunction); 
  	RouteModel::Node* nodeLowestSum = open_list.back();
  	open_list.pop_back();
  	return nodeLowestSum;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
  	RouteModel::Node * tmp = current_node;
  	RouteModel::Node * tmp2 = current_node->parent;  //(*current_node).parent
  	int ctr = 0;
  	while(tmp2 != NULL) {
      	distance += tmp->distance(*tmp2);
      	path_found.push_back(*tmp);
      	tmp = tmp->parent;
      	tmp2 = tmp2->parent;
      	std::cout << "CF: " << ctr++ << " : " << tmp << " : " << tmp2 << std::endl;
    }
	path_found.push_back(*tmp);// reverse std::reverse 
  	std::reverse(std::begin(path_found), std::end(path_found));
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
  	int ctr = 0;
    //RouteModel::Node *current_node = nullptr; //initialize current node 
	RouteModel::Node *current_node = start_node; //initialize current node 
    // TODO: Implement your solution here.
  	AddNeighbors(current_node);// expand check if u got to the final node before expanding the next neighbour node 
    RouteModel::Node * nodeWithLowestSum;
  	std::cout << start_node->x << " : " << start_node->y << " : " << end_node->x << " : " << end_node->y << std::endl;
    while(open_list.size() > 0) {    //foundn the end node or we searched everywhere and could not find it if i hit the final node srtop 	
      	if(nodeWithLowestSum == end_node) break;
      	//AddNeighbors(nodeWithLowestSum);
        nodeWithLowestSum = NextNode();
      	std::cout << nodeWithLowestSum->x << " : " << nodeWithLowestSum->y << std::endl;
      	std::cout << ctr << std::endl;
      	ctr++;
    }
    ConstructFinalPath(nodeWithLowestSum);
  	//ConstructFinalPath(nodeWithLowestSum);// if i found the final path
}