#include<vector>
#include<iostream>
#include <cstdlib>
#include "graph.hpp"
#include "kruskal.hpp" 
using namespace std;

int main()
{
	graph g1(5);
	g1.join({1, 2, 1});
	g1.join({2, 3, 7});
	g1.join({3, 4, 4});
	g1.join({4, 1, 9});
	g1.join({1, 5, 5});
	g1.join({2, 5, 2});
	g1.join({3, 5, 7});
	g1.join({4, 5, 1});
	g1.sort_edges(compare);
	graph g2 = KruskalsMinimumSpaningTree(g1) ;
	for(int i =0;i<g2.edge_degree();i++)
	{
		cout<<g2.E[i].src<<" "<<g2.E[i].dest<<" "<<g2.E[i].weight<<endl ;
	}
}