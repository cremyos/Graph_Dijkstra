#include <stdio.h>  
#include <malloc.h>  
#include <string.h>

int node_count=0,weight=0,edge_count=0;
//存放最短路径的边元素
typedef struct edge{
        int vertex;
		int value;
        struct edge* next;
}st_edge;

void createGraph(int (*edge)[node_count], int start, int end, int value);  
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

	printf("please input the number of node and edge: ");
	scanf("%d %d",&node_count,&edge_count);

	//动态创建存放边的二维数组 
	int (*edge)[node_count] = (int (*)[node_count])malloc(sizeof(int)*node_count*node_count);  
 
	memset(edge,0,sizeof(edge));
	//存放顶点的遍历状态，0：未遍历，1：已遍历
	vertexStatusArr = (int*)malloc(sizeof(int)*node_count);  
	for(i=0;i<node_count;i++){  
		vertexStatusArr[i] = 0;  
	}  

	printf("after init:\n");  
	displayGraph(edge); 

	//创建图 
	for(i=0;i<edge_count;i++)
	{
		printf("please input the node pair: ");  
		scanf("%d %d %d",&node_pair1,&node_pair2,&weight); 
		createGraph(edge,node_pair1,node_pair2,weight);
	}

	printf("after create:\n");  
	displayGraph(edge);

	st_edge** path = NULL;
	//存储最短路径的权值
	/*
	shortestPath[0] = 0;
	shortestPath[1] = 8;
	shortestPath[2] = 12;
	从顶点0到0的路径是0
	从顶点0到1的路径是8
	从顶点0到2的路径是12
	*/
	int* shortestPath = NULL;
	//从顶点0开始寻找最短路径
	int startVertex = 0;
	//最短路径
	dijkstra(edge, &path, &shortestPath, startVertex, vertexStatusArr);
	printf("the path is:\n");
	displayPath(path,startVertex,shortestPath);

	free(edge);  
	free(path);  
	return 0;  
}  
//创建图 
void createGraph(int (*edge)[node_count], int start, int end, int value){  
        edge[start][end] = value;  
}  
//打印存储的图
void displayGraph(int (*edge)[node_count]){  
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
//打印最短路径
void displayPath(st_edge** path, int startVertex,int* shortestPath){
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
//最短路径
void dijkstra(int (*edge)[node_count], st_edge*** path, int** shortestPath, int startVertex, int* vertexStatusArr){
	//初始化最短路径
	*path = (st_edge**)malloc(sizeof(st_edge*)*node_count);
        int i,j;
    for(i=0;i<node_count;i++){
		if(i == startVertex){
			st_edge* e = (st_edge*)malloc(sizeof(st_edge));
			e->vertex = startVertex;
			e->value = 0;
			e->next = NULL;
			(*path)[i] = e;
		}else{
            (*path)[i] = NULL;
		}
    }
	//初始化最短路径的权值
	*shortestPath = (int *)malloc(sizeof(int)*node_count);
	for(i=0;i<node_count;i++){
		if(i == startVertex){
			(*shortestPath)[i] = 0;
		}else{
			(*shortestPath)[i] = -1;
		}
	}
	//从顶点0开始，则顶点0就是已访问的 
	vertexStatusArr[startVertex] = 1;  
 
    int shortest, distance,start, end, edgeValue, vNum = 1;	
	//如果还顶点还没有访问完
	while(vNum < node_count)
	{
		shortest = 9999;
		for(i=0;i<node_count;i++)
		{  
			//选择已经访问过的点
			if(vertexStatusArr[i] == 1)
			{  
				for(j=0;j<node_count;j++)
				{  
					//选择一个没有访问过的点  
					if(vertexStatusArr[j] == 0)
					{  
						//选出一条value最小的边
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
		//将点设置为访问过 
		vertexStatusArr[end] = 1;   
		//保存最短路径权值
		(*shortestPath)[end] = shortest;
		//保存最短路径
		createPath(*path, startVertex, start, end, edgeValue); 
		
		//if(end==node_count-1)
			//break;
	}  
}

//返回从startVertex到新的顶点的距离
int getDistance(int value, int startVertex, int start, int* shortestPath){
	if(start == startVertex){
		return value;
	}else{
		return shortestPath[start] + value;
	}
}

//保存最短路径
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

