#include <iostream>
#include <queue>
#include <string>
#include <stdlib.h>
#include <functional>
#include <vector>
#include <math.h>
using namespace std;

struct pos{
	int x;
	int y;
	struct pos operator=(struct pos X){
		x = X.x;
		y = X.y;
	}
};
typedef struct pos pos;

int man_dis(pos a, pos b);

const int NB_DIR = 4;
const int DIM = 10;
const char START = 'S';
const char WALL = 'W';
const char END = 'E';
const char VISIT = '-';
const char PATH = '@';
const char SPACE = '.';
const int dir[4][2]={{-1,0},{0,1},{1,0},{0,-1}};//up right down left
pos **pre_pos;//store previous pos
int **dis;
pos start, end;

struct cmp_for_min//cmp func_struct for min_heap
{
	bool operator()(const struct pos &a, const struct pos &b)
	{
		return (dis[a.x][a.y] + man_dis(a,end)) >= (dis[b.x][b.y] + man_dis(b,end));	
	}
};

const string ori_map[]={
	"....................",
	"...WWWW.............",
	"......W.............",
	"......W.............",
	"..S...W.......E.....",
	"......W.............",
	"......W.............",
	"......W.............",
	"...WWWW.............",
	"...................."
	};

void print_map(const string map[]) 
{
	for(int i=0; i<DIM; i++){
		cout<<map[i]<<endl;
	}
}

void share_var_init()//malloc source fir dis[][] and pre_pos
{
	pre_pos = new pos*[DIM];	
	dis = new int*[DIM];

	for(int i=0; i<DIM; i++)
	{	
		pre_pos[i] = new pos[ori_map[i].length()];
		dis[i] = new int[ori_map[i].length()];
	}
}

template <class T>
void share_var_delete(T **a)
{
	int i=0;
	for(i=0; i<DIM; i++)
	{
		delete[] a[i];
	}
	delete[] a;
}

void map_init(string map[])
{
	for(int i=0; i<DIM; i++)
	{	
		map[i] = ori_map[i];
		for(int j=0; j<ori_map[i].length(); j++)
		{
			if(ori_map[i][j] == START)
			{
				start.x = i;
				start.y = j;
			}
			if(ori_map[i][j] == END)
			{
				end.x = i;
				end.y = j;
			}
			dis[i][j] == -1;		
		}
	}
}

void back_trace(string map[], pos cur_pos){
	int cur_x = pre_pos[cur_pos.x][cur_pos.y].x;
	int cur_y = pre_pos[cur_pos.x][cur_pos.y].y;
	int pre_x = 0;
	int pre_y = 0;
	
	while( map[cur_x][cur_y] != START){
		
		pre_x = pre_pos[cur_x][cur_y].x;
		pre_y = pre_pos[cur_x][cur_y].y;
		if(pre_x == (cur_x - 1))//pre is up
			map[cur_x][cur_y] = 'v';
		else if(pre_x == (cur_x + 1))//pre is down
			map[cur_x][cur_y] = '^';
		else if(pre_y == (cur_y - 1))//pre is left
			map[cur_x][cur_y] = '>';
		else//pre is right
			map[cur_x][cur_y] = '<';
		//move to pre pos		
		cur_x = pre_x;
		cur_y = pre_y;
	}
}

int BFS_find(string map[])
{
	queue<pos> pos_q;
	pos cur;
	dis[start.x][start.y]=0;
	pos_q.push(start);
		
	int i=0;
	
	while(!pos_q.empty()){
		cur = pos_q.front();
		if( map[cur.x][cur.y] != START )
			map[cur.x][cur.y] = VISIT;//visited
		pos_q.pop();
		for(i=0; i<NB_DIR; i++){//test four direction
			pos tmp;
			tmp.x = cur.x + dir[i][0];	
			tmp.y = cur.y + dir[i][1];
			if(tmp.x < 0 || tmp.x >= DIM ||
			   tmp.y < 0 || tmp.y >= ori_map[0].length() ||
			   ( map[tmp.x][tmp.y] != SPACE && map[tmp.x][tmp.y] != END ))//check if current pos is valid
			{
				continue;
			}
			dis[tmp.x][tmp.y] = dis[cur.x][cur.y] + 1;//update distance to start pos
			pre_pos[tmp.x][tmp.y] = cur;//update previous pos
			
			if(map[tmp.x][tmp.y] == END )
			{
				back_trace(map, tmp);
				return 1;
			}
			pos_q.push(tmp);
		}
	}
	return 0;
}

int man_dis(pos a, pos b){
	//return sqrt(pow(abs(a.x - b.x), 2) +pow(abs(a.y - b.y), 2));
	return abs(a.x - b.x) + abs(a.y - b.y);
}

int ASTAR_find(string map[])
{
	priority_queue<pos, vector<pos>, cmp_for_min> pos_q;//min heap implemented by priority_queue 
	pos cur, cur_min;
	dis[start.x][start.y]=0;
	pos_q.push(start);
		
	int i=0;
	
	while(!pos_q.empty()){
		cur = pos_q.top();
		if(map[cur.x][cur.y] != START)
			map[cur.x][cur.y] = VISIT;	
		pos_q.pop();
		for(i=0; i<NB_DIR; i++){//test four direction
			pos tmp;
			tmp.x = cur.x + dir[i][0];	
			tmp.y = cur.y + dir[i][1];

			if(tmp.x < 0 || tmp.x >= DIM ||
			   tmp.y < 0 || tmp.y >= ori_map[0].length() ||
			   ( map[tmp.x][tmp.y] != SPACE && map[tmp.x][tmp.y] != END ))//check if current pos is valid
			{
				continue;
			}
			//only if new found pos or pos whose dis to start can be shorter, update it's pre pos and dis
			if(dis[tmp.x][tmp.y] == -1 || dis[tmp.x][tmp.y] > (dis[cur.x][cur.y] + 1))
			{
				dis[tmp.x][tmp.y] = dis[cur.x][cur.y] + 1;		
				pre_pos[tmp.x][tmp.y].x = cur.x;//update father pos
				pre_pos[tmp.x][tmp.y].y = cur.y;//update father pos
			}
			if(map[tmp.x][tmp.y] == END )
			{
				back_trace(map, tmp);
				return 1;
			}
			pos_q.push(tmp);	
			}
	}
	return 0;
}

int main(int argc, char **argv)
{
	string map[10];
	
	share_var_init();
	cout<<"ORI:\n";	
	print_map(ori_map);
	
	map_init(map);
	if(BFS_find(map) == 1)
	{
		cout<<"BFS:\n";
		print_map(map);
	}	
	else{
		cout<<"BFS (no way):\n";
		print_map(map);
	}
	
	map_init(map);
	if(ASTAR_find(map) == 1)
	{
		cout<<"A_STAR:\n";
		print_map(map);
	}	
	else{
		cout<<"A_STAR (no way):\n";
		print_map(map);
	}


//free source	
	share_var_delete(dis);
	share_var_delete(pre_pos);
	return 0;	
}




