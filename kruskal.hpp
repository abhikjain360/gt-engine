#pragma once

#include<vector>
#include <cstdlib>
#include "graph.hpp"

using namespace std;

bool compare(const edge &e1, const edge &e2) { return e1.weight < e2.weight; }
void makeSet(int v,vector<int> &parent)
{
	parent[v] = v ;
}
int findSet(int v,vector<int> &parent)
{
	if(v==parent[v])
	{
		return v ;
	}
	v = findSet(parent[v],parent) ;
	return v ;
}
bool unionSet(int a,int b,vector<int> &parent,vector<int> &depth)
{
	a = findSet(a,parent);
	b = findSet(b,parent);
	if(a!=b)
	{
		if (depth[a]<depth[b])
		{
			swap(a,b) ;
		}
		parent[b] = a ;
		depth[a] +=depth[b] ;
		return true ;
	}
	return false ;

}
graph KruskalsMinimumSpaningTree(graph &g1)
{	
	int n = g1.degree() ;
	graph mst(n) ;
	vector<int> parent;
	parent.resize(n,-1);
	vector<int> depth;
	depth.resize(n,1) ;
	for(int i = 0;i<n;i++)
	{
		makeSet(i,parent) ;
	}
	for (int i = 0; i < g1.edge_degree(); i++)
	{
		if(unionSet(g1.E[i].src,g1.E[i].dest,parent,depth))
		{
			mst.join({g1.E[i].src,g1.E[i].dest,g1.E[i].weight}) ;
		}
	}


	return mst;
}
