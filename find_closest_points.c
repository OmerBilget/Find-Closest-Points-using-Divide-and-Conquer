

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct point{
	int x; //x coordinate
	int y; //y coordinate
}POINT;

//struct that keeps coordinates and distance closest pair
typedef struct pair{
	POINT p1; //Point 1
	POINT p2; //Point 2
	int dist; //distance calculated using integer arithmetics
}PAIR;

//Function Declarations
POINT* read_from_file(char*,int*);
void sort_by_x(POINT*,int);
int calculate_distance(int,int,int,int);
void find_min_dist_brute_force(POINT*,int,int,PAIR*,int);
void extended_search(POINT*,int,int,PAIR*);
void divide_and_conquer_pair(int,int,POINT*,PAIR*,int);
void find_closest_points();


//main function
int main(){
	find_closest_points();
	return 0;
}

//read coordinates from text and generate struct array
//return array of POINT struct
POINT* read_from_file(char* filename,int*len){
	FILE* fp=fopen(filename,"r"); 
	int i;//loop variable
	char buffer[20]; //buffer string used to find point count
	int counter=0;//point count
	POINT* list_of_points;//struct array that keeps points
	if(fp==NULL){
		return NULL;
	}
	
	//find point number
	//count lines
	while(fgets(buffer,20,fp)!=NULL){
		counter+=1;
	}
	
	//dinamic memory allocation for struct array 
	
	list_of_points=(POINT*)malloc(counter*sizeof(POINT));
	
	rewind(fp);
	//read points
	for(i=0;i<counter;i++){
		fscanf(fp,"%d %d\n",&list_of_points[i].x,&list_of_points[i].y);
	}
	fclose(fp);
	*len=counter; //keep point length
	return list_of_points;
	
}


//sort coordinates by x values ascending order
void sort_by_x(POINT* list_of_points,int len){
	//bubble sort
    int i,j;//loop variable
    for(i=0;i<len-1;i++) {
   		for(j=0;j<len-i-1;j++){
   			if(list_of_points[j].x > list_of_points[j+1].x){
              	POINT tmp=list_of_points[j];
              	list_of_points[j]= list_of_points[j+1];
               	list_of_points[j+1]=tmp;
            }
		}
  	}   
}


//when point count 2 or 3 brute force search all distances 
//update PAIR struct that keeps closest points 
void find_min_dist_brute_force(POINT* list_of_points,int start_index,int end_index,PAIR* p,int len){
	int i,j;//loop variables
	int dist;//assign distance to this variable
	
	for(i=start_index;i<end_index;i++){
		for(j=i+1;j<=end_index;j++){
			dist=calculate_distance(list_of_points[i].x,list_of_points[i].y,list_of_points[j].x,list_of_points[j].y);
			if(p->dist>dist){
				p->p1=list_of_points[i];
				p->p2=list_of_points[j];
				p->dist=dist;
			}
		}
	}
	//extended search for points in different regions
	//search left of the border
	extended_search(list_of_points,start_index,len,p);
	//search right of the border
	extended_search(list_of_points,end_index,len,p);
	
}

//extend the search for points in different regions 
void extended_search(POINT* list_of_points,int start_index,int len,PAIR* p){
	int start=start_index;//search start index
	int end=start_index;//search end index
	int range=sqrt(p->dist);//search min_dist range
	int i,j;//loop variables
	int dist;//assign distance to this variable
	
	//find start and end index according to defined range
	while(start>0 && (list_of_points[start_index].x - list_of_points[start].x <range)){
		start-=1;
	}
	while(end<len && (list_of_points[end].x - list_of_points[start_index].x < range)){
		end+=1;
	}
	
	//calculate distance and find min distance
	//update PAIR struct
	for(i=start;i<end;i++){
		for(j=i+1;j<=end;j++){
			dist=calculate_distance(list_of_points[i].x,list_of_points[i].y,list_of_points[j].x,list_of_points[j].y);
			if(p->dist>dist){
				p->p1=list_of_points[i];
				p->p2=list_of_points[j];
				p->dist=dist;
			}
		}
	}
}

//calculate distance between two points
//I used integer because the algoritm only needs relative distance 
//so float calculations are unnecessary
int calculate_distance(int x1,int y1,int x2,int y2){
	return ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


//recursive search of list of points 
void divide_and_conquer_pair(int start_index,int end_index,POINT* list_of_points,PAIR* closest,int point_list_size){
	int point_range=end_index-start_index+1; //point number between start index and end index
	int median;//median of given list between start index and end index
	
	//if point number 2 or 3 brute force search
	if(point_range>= 2 && point_range<=3){ 
		find_min_dist_brute_force(list_of_points,start_index,end_index,closest,point_list_size);
		return;
	}
	//calculate median
	median=(end_index+start_index)/2;
	
	//if point number is bigger than 3 divide search to two 
	divide_and_conquer_pair(start_index,median,list_of_points,closest,point_list_size);
	divide_and_conquer_pair(median+1,end_index,list_of_points,closest,point_list_size);
}


//call in main function
void find_closest_points(){
	POINT* list_of_points;//list of points
	int len;//point count
	int i;//loop variable
	PAIR p;//struct which keeps closest points
	
	
	//read and sort coordinates
	list_of_points=read_from_file("sample.txt",&len);
	if(list_of_points==NULL){
		printf("error\n");
		return;
	}
	sort_by_x(list_of_points,len);
	
	//print list of points
	printf("Points\n");
	for(i=0;i<len;i++){
		printf("p(%d,%d)\n",list_of_points[i].x,list_of_points[i].y);
	}
	printf("\n");
	
	

	p.dist=1000000;//initial distance
	divide_and_conquer_pair(0,len-1,list_of_points,&p,len);
	//print closest points to the screen
	printf("closest points\n");
	printf("p(%d,%d)-p(%d,%d)\n",p.p1.x,p.p1.y,p.p2.x,p.p2.y);
	printf("distance: %f\n",sqrt(p.dist));
	//free memory
	free(list_of_points);
}


