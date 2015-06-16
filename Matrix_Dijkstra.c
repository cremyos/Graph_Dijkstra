/*
 ************************************************
 *Name : Matrix_Dijkstra.c                      *
 *Date : 2015-06-08                             *
 *Author : sniper                               *
 *Aim : Matrix storage the graph and using the  *
 *      Dijkstra algrithm to find the way v0 to *
 *      every node.                             *
 ************************************************
 */
#include <stdio.h>  
#include <stdlib.h>
#include <malloc.h>  
#include <string.h>

int node_count=0,weight=0,edge_count=0;

/*
 *storage the shotest edge
 */
typedef struct edge{
        int vertex;
		int value;
        struct edge* next;
}st_edge;

void displayGraph(int (*edge)[node_count]); 
void displayPath(st_edge** path, int startVertex,int* shortestPath);
void dijkstra(int (*edge)[node_count], st_edge*** path, int** shortestPath, int startVertex, int* vertexStatusArr);
int getDistance(int value, int startVertex, int start, int* shortestPath);
void createPath(st_edge **path, int startVertex, int start, int end, int edgeValue);

int main()
{
	int i; 
	int node_pair1,node_pair2;
	int* vertexStatusArr;
	int* shortestPath = NULL;	
	int startVertex = 0;

	printf("please input the number of node and edge: ");
	scanf("%d %d",&node_count,&edge_count);

	int (*edge)[node_count] = (int (*)[node_count])malloc(sizeof(int)*node_count*node_count);  
 
	memset(edge,0,sizeof(edge));
	
	/*
	 *storage the visit status of node
	 *0 not vist 1 visit
	 */
	vertexStatusArr = (int*)malloc(sizeof(int)*node_count);  
	for(i=0;i<node_count;i++){  
		vertexStatusArr[i] = 0;  
	}  

	printf("after init:\n");  
	displayGraph(edge); 

	/*
	 *Create the graph
	 */
	for(i=0;i<edge_count;i++)
	{
		printf("please input the node pair: ");  
		/*
		 *node_pair1 node_pair2 two side of edge and 
		 *the weight means the weight of edge
		 */
		scanf("%d %d %d",&node_pair1,&node_pair2,&weight); 
		edge[node_pair1][node_pair2] = weight;
	}
	printf("after create:\n");  
	displayGraph(edge);

	st_edge** path = NULL;	
	
	/*
	 *from the v0 to find the way
	 */
	startVertex = 0;

	/*
	 *Dijkstra algrithm
	 */
	dijkstra(edge, &path, &shortestPath, startVertex, vertexStatusArr);
	printf("***********************************************\n");

	printf("the path is:\n");
	displayPath(path,startVertex,shortestPath);

	free(edge);  
	free(path);  
	return 0;  
}  

/*
 *print the graph
 */
void displayGraph(int (*edge)[node_count])
{  
	int i,j;  
	for(i=0;i<node_count;i++)
	{  
		for(j=0;j<node_count;j++)
		{  
			printf("%d ",edge[i][j]);  
		}  
		printf("\n");  
	}  
}

/*
 *print the shortest path
 */
void displayPath(st_edge** path, int startVertex,int* shortestPath)
{
	int i;
	st_edge* p;
	for(i=1;i<node_count;i++)
	{
		if(shortestPath[i]==-1)
		{
			printf("No way to %c node !\n",i+'A');
			continue;
		}
		printf("Path from %c to %c:",startVertex+'A',i+'A');
		p = *(path+i);
		while(p != NULL)
		{
			printf("%d(%d) ",p->vertex,p->value);
			p = p->next;
		}
		printf("\n");
		printf("the count is:%d\n",shortestPath[i]);
	}
}

/*
 *Dijkstra find the shortest path
 */
void dijkstra(int (*edge)[node_count], st_edge*** path, int** shortestPath, int startVertex, int* vertexStatusArr)
{
	int i=0,j=0;
	int shortest=0, distance=0,start=0, end=0, edgeValue=0, vNum = 1;

	/*
	 *init the shortest path
	 */
	*path = (st_edge**)malloc(sizeof(st_edge*)*node_count);        
    for(i=0;i<node_count;i++)
	{
		if(i == startVertex)
		{
			st_edge* e = (st_edge*)malloc(sizeof(st_edge));
			e->vertex = startVertex;
			e->value = 0;
			e->next = NULL;
			(*path)[i] = e;
		}
		else
            (*path)[i] = NULL;
    }
	/*
	 *Init the shortest path weight
	 */
	*shortestPath = (int *)malloc(sizeof(int)*node_count);
	for(i=0;i<node_count;i++){
		if(i == startVertex){
			(*shortestPath)[i] = 0;
		}else{
			(*shortestPath)[i] = -1;
		}
	}
	/*
	 *from the v0 to visit
	 */
	vertexStatusArr[startVertex] = 1;  
 
	/*
	 *find the way v0 to other node
	 */
	while(vNum < node_count)
	{
		shortest = 9999;
		for(i=0;i<node_count;i++)
		{  
			/*
			 *choice the node has been visited
			 */
			if(vertexStatusArr[i] == 1)
			{  
				for(j=0;j<node_count;j++)
				{  
					/*
					 *choice the node has not been visited
					 */ 
					if(vertexStatusArr[j] == 0)
					{  
						/*
						 *choice the node that distance is minist
						 */
						if(edge[i][j] != 0 && (distance = getDistance(edge[i][j], startVertex, i,  *shortestPath)) < shortest)
						{  
							shortest = distance;  
							edgeValue = edge[i][j];
							start = i;  
							end = j;  
						}  
					}  
				}  
			}  
		} 	
		vNum++; 
		if(shortest == 9999)
			continue;	 
		/*
		 *make the node set visited
		 */
		vertexStatusArr[end] = 1;  
		/*
		 *visit the shortest path weight
		 */ 
		(*shortestPath)[end] = shortest;
		/*
		 *create the shortest path
		 */
		createPath(*path, startVertex, start, end, edgeValue); 
	}  
}

/*
 *get the distance from the v0 to new node
 */
int getDistance(int value, int startVertex, int start, int* shortestPath){
	if(start == startVertex){
		return value;
	}else{
		return shortestPath[start] + value;
	}
}

/*	
 *Save the shortest path
 */
void createPath(st_edge **path, int startVertex, int start, int end, int edgeValue){
	if(start == startVertex){
		st_edge* newEdge = (st_edge*)malloc(sizeof(st_edge));
		newEdge->vertex = end;
		newEdge->value = edgeValue;
		newEdge->next = NULL;
		
		st_edge** p = path + end;
		while((*p) != NULL){
			p = &((*p)->next);
		}
		*p = newEdge;
	}else{
		st_edge** pCopySrc = path + start;
		st_edge** pCopyDes = path + end;
		st_edge* newEdge = NULL;
		while((*pCopySrc) != NULL){
			newEdge = (st_edge*)malloc(sizeof(st_edge));
			*newEdge = **pCopySrc;
			newEdge->next = NULL;
			*pCopyDes = newEdge;
			pCopySrc = &((*pCopySrc)->next);
			pCopyDes = &((*pCopyDes)->next);
		}
		newEdge = (st_edge*)malloc(sizeof(st_edge));
		newEdge->vertex = end;
		newEdge->value = edgeValue;
		newEdge->next = NULL;
		*pCopyDes = newEdge;
	}
}

