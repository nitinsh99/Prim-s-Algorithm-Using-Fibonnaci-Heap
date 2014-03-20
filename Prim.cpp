#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
#define MAX 100000;
using namespace std;


struct fnode			//node structure for a fibonacci heap
{
	int value;    //to be used to check for the priority
	int id;			//id of the node	
	fnode* child;		//to point to the child
	bool cas;			//cascading cut check
	fnode* left;			//to point to next node
	fnode* right;			//to point to previous node
	int post;				//an extra variable to maintain post for applying prim's
};	


struct node					//node structure for simple adjacency scheme
{
    int id;				//stroring node id
	int w;				//weight of the node b/w id and its parent in adj list	
    struct node* next;		// to point to the next neighnour of parent 
};

struct adjlist					//sturcure for the nodes in main chian of adj list
{
	int id;					//to store id of nodes	
    struct adjlist *next; 	//to point to next node in the main chian of adj list
	struct node* next2; 		//to point to its neighbour since it's a 2d chain
};


clock_t Start, Time; 				//for performance analysis
adjlist* head = new adjlist;		//will point to the chain of nodes in adj list
fnode* fhead = new fnode;			//point the min elment in fibonacci heap
int gvertex;					//global vertex to store number of vertices 
int gcall=0;					//it's basically a counter to count number of time removemin is called in fib heap


void initheap();			//initializing the fib heap circular linked list
int removemin();			//called to return the minimum element pointed by fhead in fib heap
void f_scheme();			//this is the main call to rum prim's using f-scheme
void s_scheme();				//main call to run prim's using s-scheme
int getw(int i,int j,adjlist*& head);			//this returns the weight in  the s-scheme, arugument provide are nodes id and pointer to the head to the chian of node id's in adj list
void add(int a,int b,int c,adjlist*& head);				//this will add node c as neibhour of node a with weight of a-b = c in the s-scheme
void f_init();										//this is called to populate cicular linked list created in initheap() with the edge lenght in accordance to prim's algorithm
void random(int n,int e);				//to generate a random data file connecting a connected graph with n nodes and e edges which will be used as input





void initheap()
{
	
	fnode* fcurrent;    
	fcurrent = fhead;   //fcurrent initially pointing to the head of the doubly circular linked list in the fb heap
	
	
	for(int i=0;i<gvertex;i++)   //initializing the fp heap nodes circular linked list
	{
		
			if(i==0)    //case when populating node with id 0
			{
			 	fnode* temp = new fnode;
				fcurrent->value=MAX;
				fcurrent->cas = false;
				fcurrent->id=i;
				fcurrent->child=NULL;
				fcurrent->right=temp;
				fcurrent->left = NULL;
				fcurrent->post=MAX;
				temp->left = fcurrent;
				fcurrent=temp;
				temp=NULL;	
			}
			else if(i!=0 && i!=gvertex-1)    //case when populating intermediate nodes
			{
				fnode* temp = new fnode;
				fcurrent->value=MAX;
				fcurrent->cas = false;
				fcurrent->id=i;
				fcurrent->child=NULL;
				fcurrent->post=MAX;
				fcurrent->right=temp;
				temp->left = fcurrent;
				fcurrent=temp;
				temp=NULL;
			}
			else						//case when populating node with id gvertex-1 or last node
			{
				fcurrent->value=MAX;
				fcurrent->cas = false;
				fcurrent->id=i;
				fcurrent->post=MAX;
				fcurrent->child=NULL;
				fcurrent->right=fhead;
				fhead->left = fcurrent;
				
			}
			
			
	}	

	
}

int removemin()
{
	fnode* temp1;
	fnode* currentmin;
	temp1 = fhead;
	currentmin = fhead;				//storing current minimum to delete it in future
	int store;								//storing the current minimun which is nothing but the edge wegiht of next edge to be added in the mst 
	store = fhead ->value;		//storing minvalue before this node is deleted
	//cout<<store;
	int nodes = gvertex-gcall;
	int min =100000;
	while(nodes--)					//nodes is number of remaining nodes
	{
		
		if(temp1->post == gcall+1)				//searching for the node with next higher post 
		{
			fhead = temp1;						//reassigning minpoint
		}
		temp1= temp1->right;
		
	
	}
	
		currentmin->left->right = currentmin->right;				//deleting the current minimum from the doubly circular linked list
		currentmin=NULL;
	
	return store;
	
	
}

void f_init()				
{	
	int v,e,x;
	cin>>v;				//taking nodes or vertex input from the file 
	cin>>e;					//taking number of edges from file
	gvertex=v;				//populating global veretex =v so that this can be used in other functions
	adjlist* current;
	current=head;

	

	
	for(int i=0;i<v;i++)					//this is basically creating an adj list with v veretx which will be used to populate fibonacci heap
	{
		current->id=i;
		adjlist* temp = new adjlist;
		current->next=temp;
		current=temp;
		current->next=NULL;
		
	}
	
	int a,b,c;
	for(int i=0;i<e;i++)
	{
		cin>>a;
		cin>>b;
		cin>>c;
		add(a,b,c,head);
			
	}

	
}



void f_scheme()
{
	f_init();
	initheap();
	int ftrack[gvertex][2];
	int maini=0,mainj=0,tempj=0,minou=1000;
	int counter=gvertex-1;
	int counter1=gvertex;
	int counter2=gvertex;
	int cost=0;
	int index = 1;
	fnode* temp2;
	temp2= fhead;      //fb heap head pointer
	temp2->value=0;    //setting it's value to 0 (distance to itself)
	temp2->post =0;    //setting it's priority to 0 (higest or first to be addes in the mst)
	
	
	
	for(int k=1;k<gvertex;k++)
	{
		ftrack[k][1]=0;     //nodes other then id 0 are not in mst
	}
	
	ftrack[0][1]=1;   //only node 0 is in the mst
	
	while(counter--)    //running prim's mst algorithm
	{
		minou=1000;
		
		for(int i=0;i<gvertex;i++)    //loop upto global vertex
		{
			
			int minin=1000;
			if(ftrack[i][1] == 1)     //ftrack to track which nodes id are already in the mst 1 for in o for out
		    { 
				for(int j=0;j<gvertex;j++)
				{
					if(ftrack[j][1] == 0)
					{
						if(getw(i,j,head)<minin)    //getting the weight from adj list
						{
							minin=getw(i,j,head);
							tempj=j;
							
						}
					}
				}
				//cout<<"\n";
				
			}
			
			if(minin<minou)
			{
				minou=minin;
				maini=i;
				mainj=tempj;
				cout<<"\n";
				
									
			}
			
		}
		ftrack[mainj][1]=1;   //particular id which is equal to main j has been added to the mst
		
			cout<<maini<<"\t"<<mainj;
			
			while(counter1--)
			{
				if(temp2->id == mainj)     //searching for the node having id equal to mainj which prim's has selected the id of next node to be added
				{
					temp2->value = getw(maini,mainj,head);    //storing the wegiht of newly added edge in fb heap correspoinding to the id of newly added vertex
					temp2->post = index;	//post is the parameter for storing the priority in which nodes will be accessed during remove min
					index++;  					//just a variable to populate post each time
					break;
				}
				temp2 = temp2->right;
				
			}
			//cout<<temp2->value;
			temp2=fhead;
			counter1=gvertex;
		
		
	}
	
	Start = clock(); 
	while(counter2--)
	{	
		cost = cost + removemin();			//performing remove min number of nodes times
		++gcall;
  	}

	cout<<"\n cost = "<<cost;
	Time = clock()-Start;
	//cout<<"\nTime"<<Time;
	//cout<<"\n";
	
}


int getw(int i,int j,adjlist*& head)
{
	adjlist* temp1 = head;				//temp to point to main chain having all nodes it
	while(temp1->next != NULL)
		{
			if(temp1->id == i)					//traversing to the node having id=i
			{
				//cout<<"matched";
				node* temp;
				temp=temp1->next2;
				while(temp!=NULL)				
				{
					//cout<<temp->id;
					if(temp->id == j)			//traversing the neighbour list of i to find j 
					{
						//cout<<"matched";
						return temp->w;				//returing the weight of the edge between i and j
					}
					temp= temp->next;
					
				}
				
			}
			temp1 = temp1->next;
		}
		return 10000;				//if no edge is found returning infinity or a very large number compared to edges weight
}

void add(int a,int b,int c,adjlist*& head)
{
		adjlist* temp1=head;
		while(temp1->next!=NULL)
		{
			if(temp1->id==a)					//traversing the main chain to find the node with id =i
			{
				//cout<<temp1->id;
				//cout<<"\n";
				node* temp2;
				node* temp3;
				temp2=temp1->next2;				
				if(temp2 == NULL)			 //if neigbhour list is null 
				{	
					temp3 = new node;				//creating a new node with id j in neigbour list of i and putting the weight of edges between a and b in this node
					temp3->id=b;
					temp3->w=c;
					
					temp2=temp3;
					temp2->next=NULL;
					temp3=NULL;
					//cout<<temp2->id;
					//cout<<temp2->w;
					temp1->next2=temp2;
				}
				else
				{
					while(temp2->next != NULL)			//if neigbour list is not null then raversing the neighbour list of i.
					{
						temp2=temp2->next; 
					}
					temp3 = new node;
					temp3->id=b;
					temp3->w=c;
					temp2->next=temp3;
					temp2=temp3;
					temp2->next=NULL;
					temp3=NULL;
					//cout<<temp2->id;
					//cout<<temp2->w;
				}
				
			}
			temp1 = temp1->next;
		}
		temp1=head;
		while(temp1->next!=NULL)				//repeating the above procure now with main node as b and a as it's neighbour since 0 1 15 will 
		{											//be stored as 1 as neighnour of 0 with weight 15 and then 0 as neighbour of 1 with weight = 15.
			if(temp1->id==b)
			{
				//cout<<temp1->id;
				//cout<<"\n";
				node* temp2;
				node* temp3;
				temp2=temp1->next2;
				if(temp2 == NULL)
				{
					temp3 = new node;
					temp3->id=a;
					temp3->w=c;
					
					temp2=temp3;
					temp2->next=NULL;
					temp3=NULL;
					//cout<<temp2->id;
					//cout<<temp2->w;
					temp1->next2=temp2;
				}
				else
				{
					while(temp2->next != NULL)
					{
						temp2=temp2->next; 
					}
					temp3 = new node;
					temp3->id=a;
					temp3->w=c;
					temp2->next=temp3;
					temp2=temp3;
					temp2->next=NULL;
					temp3=NULL;
					//cout<<temp2->id;
					//cout<<temp2->w;
				}
				
			}
			temp1 = temp1->next;
		}
}



void s_scheme()
{
	int v,e,x;					//same thing as above function taking input of nodes and edges but for s-scheme in this case
	cin>>v;
	cin>>e;
	int track[v][2];				//this 2d array will keep track of nodes which are added in the mst while running prim's
	track[0][1] = 1;							//only node with 0 is in mst initially
	adjlist* current;
	current=head;
	int maini=0,mainj=0,tempj=0,minou=1000,counter=v;			//some variables to be used during  prim's
	int cost=0;
	
	
	for(int k=1;k<v;k++)
	{
		track[k][1]=0;						//rest all other nodes are not in mst
	}
	
	for(int i=0;i<v;i++)						//now creating adj list, first creating a chain of nodes with id's from 0 to n
	{
		current->id=i;
		adjlist* temp = new adjlist;
		current->next=temp;
		current=temp;
		current->next=NULL;
		
	}
	
	int a,b,c;
	for(int i=0;i<e;i++)					//next reading from file their neighnour and edge weight b/w them and for each reading two enteries will be stored in adj list
	{
		cin>>a;
		cin>>b;
		cin>>c;
		add(a,b,c,head);			//calling add fucntion correspondign to node with id a and b with edge weight c.
			
	}
	

	
	while(--counter)				//prim's start here, in this case for teh f-scheme
	{
		minou=1000;
		
		for(int i=0;i<v;i++)
		{
			
			int minin=1000;
			if(track[i][1] == 1)			//track checking for nodes already in the mst
		    { 
				for(int j=0;j<v;j++)
				{
					if(track[j][1] == 0)		//chekcing for nodes not in the mst
					{
						if(getw(i,j,head)<minin)			
						{
							minin=getw(i,j,head);				
							tempj=j;				//searching for new node having least edge weight from node already in the mst
							
						}
					}
				}
				//cout<<"\n";
				
			}
				
			if(minin<minou)
			{
				minou=minin;
				maini=i;
				mainj=tempj;					//maini and mainj are the pair of nodes found to have the least weighted edge 
				cout<<"\n";
				
									
			}
			
		}
		track[mainj][1]=1;				//adding node with id mainj in the tree
		
			cout<<maini<<"\t"<<mainj;
			cost = cost+getw(maini,mainj,head);			//updateing the cost of mst
				
		
	}
	
	cout<<"\n";
	cout<<"cost of mst = ";				//printing the cost of mst
	cout<<cost;
	cout<<"\n";
	
}


void random(int n,int e)							//random function print a connected graph in a file data_random with n nodes and e edges as instructed
{
	ofstream fout2;
	fout2.open("data_random.txt");		//opening file
	
	for(int i=0;i<=e;i++)			
	{
		if(i==0)
		{
			fout2<<n;
			fout2<<"\t";
			fout2<<e;
			fout2<<"\n";
		}
		else
		{
			fout2<<i-1;
			fout2<<"\t";
			fout2<<i;
			fout2<<"\t";
			fout2<<rand()%100+1;			//writing random data to the file
			fout2<<"\n";
		}
	}
	fout2.close();			//closing file
}

int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cout<<"please enter mode and file name as arguments\n";
		return 0;
	}
	
	if(strcmp(argv[1], "-r") == 0)
	{
		cout<<"he";
		int n =atoi(argv[2]);			//taking input of the number of nodes
		int d =atoi(argv[3]);			//density	
		int e = (n*n)*d/100;			//calculating edges based on nodes and density
		random(n,e);					//creating a file with random connected graph
			
			
	//Start = clock(); 
	//s_scheme();							//calling s_scheme on randomly generated graph
	//Time = clock()-Start;
	//cout<<"\nTime"<<Time;
		cout<<"random data generated in data_random.txt, please run this using f or s scheme";
		cout<<"\n";
		return 0;	
		
	}
	
	if(strcmp(argv[1], "-s") == 0)
	{
		Start = clock(); 
		s_scheme();						//calling s_scheme on user input file
		Time = clock()-Start;
		//cout<<"\nTime"<<Time;
		//cout<<"\n";
	}
	
	else if(strcmp(argv[1], "-f") == 0)
	
	{
		f_scheme();						//calling f_scheme on user input file
		
	}
	
    else{}	
	
	


	
	return 0;
}
