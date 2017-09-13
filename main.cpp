#include <iostream>
#include <queue>
#include <string>
#include <stdlib.h>
using namespace std;

struct pos{
	int x;
	int y;
};
typedef struct pos pos;

const int NB_DIR = 4;
const int DIM = 10;
const char START = 'S';
const char WALL = 'W';
const char END = 'E';
const char VISIT = '-';
const char PATH = '@';
const int dir[4][2]={{-1,0},{0,1},{1,0},{0,-1}};//up right down left
pos **pre_pos;

const string ori_map[]={
	"....................",
	"...WWWW.............",
	"......W.............",
	"......W.............",
	"..S...W.............",
	"......W.............",
	"......W.............",
	"......W.............",
	"...WWWW.............",
	".................E.."
	};

void print_map(const string map[], 
		const string map_name)
{
	cout<<map_name<<":"<<endl;
	for(int i=0; i<DIM; i++){
		cout<<map[i]<<endl;
	}
}

void map_init(string map[], pos &start, pos &end)
{
	pre_pos = new pos*[DIM];
	
	for(int i=0; i<DIM; i++)
	{	
		pre_pos[i] = new pos[ori_map[i].length()];
		map[i] = ori_map[i];
		for(int j=0; j<ori_map[i].length(); j++)
		{
			if(ori_map[i][j] == 'S')
			{
				start.x = i;
				start.y = j;
			}
			if(ori_map[i][j] == 'E')
			{
				end.x = i;
				end.y = j;
			}
					
		}
	}
}

void back_trace(string map[], pos cur_pos){
	int cur_x = cur_pos.x;
	int cur_y = cur_pos.y;
	int pre_x = 0;
	int pre_y = 0;
	
	while( map[cur_x][cur_y] != 'S'){
		
		if(map[cur_x][cur_y] != 'E'){
			map[cur_x][cur_y] = '@';
		}
		pre_x = pre_pos[cur_x][cur_y].x;
		pre_y = pre_pos[cur_x][cur_y].y;
		cur_x = pre_x;
		cur_y = pre_y;
	}
}

int BFS_find(string map[], pos start)
{
	queue<pos> pos_q;
	pos cur;
	pos_q.push(start);
		
	int i=0;
	
	while(!pos_q.empty()){
		cur = pos_q.front();
		pos_q.pop();
		for(i=0; i<NB_DIR; i++){//test four direction
			pos tmp;
			tmp.x = cur.x + dir[i][0];	
			tmp.y = cur.y + dir[i][1];
			
			if(tmp.x < 0 || tmp.x >= DIM ||
			   tmp.y < 0 || tmp.y >= map[0].length() ||
			   map[tmp.x][tmp.y] == '-' ||
			   map[tmp.x][tmp.y] == 'W' ||
			   map[tmp.x][tmp.y] == 'S')
			{
				continue;
			}
			pre_pos[tmp.x][tmp.y].x = cur.x;//update father node pos
			pre_pos[tmp.x][tmp.y].y = cur.y;//update father node pos

			if(map[tmp.x][tmp.y] == 'E' )
			{
				back_trace(map, tmp);
				return 1;
			}
			map[tmp.x][tmp.y] = '-';//mark for visited
			pos_q.push(tmp);
		}
	}
	return 0;
}

int man_dis(pos a, pos b){
	return abs(a.x - b.x) + abs(a.y - b.y);
}

int ASTAR_find(string map[], pos start, pos end)
{
	queue<pos> pos_q;
	pos cur, cur_min;
	pos_q.push(start);
		
	int i=0;
	int first_expand = 0;
	
	while(!pos_q.empty()){
		first_expand = 0;
		cur = pos_q.front();
		pos_q.pop();
		for(i=0; i<NB_DIR; i++){//test four direction
			pos tmp;
			tmp.x = cur.x + dir[i][0];	
			tmp.y = cur.y + dir[i][1];
			
			if(tmp.x < 0 || tmp.x >= DIM ||
			   tmp.y < 0 || tmp.y >= map[0].length() ||
			   map[tmp.x][tmp.y] == '-' ||
			   map[tmp.x][tmp.y] == 'W' ||
			   map[tmp.x][tmp.y] == 'S')
			{
				continue;
			}
			pre_pos[tmp.x][tmp.y].x = cur.x;//update father node pos
			pre_pos[tmp.x][tmp.y].y = cur.y;//update father node pos
			
			if(map[tmp.x][tmp.y] == 'E' )
			{
				back_trace(map, tmp);
				return 1;
			}
			
			if(first_expand == 0){
				first_expand = 1;
				cur_min.x = tmp.x;
				cur_min.y = tmp.y;
			}
			else{
				if( ( man_dis(tmp,start) + man_dis(tmp,end) ) <= ( man_dis(cur_min, start) + man_dis(cur_min, end) ) )
				{
					cur_min.x = tmp.x;
					cur_min.y = tmp.y;
				}
			}
				
		}
		map[cur_min.x][cur_min.y] = '-';//mark for visited
		pos_q.push(cur_min);
	}
	return 0;
}

int main(int argc, char **argv)
{
	string map[10];
	pos start, end;
	
	print_map(ori_map, "ORI");
	
	map_init(map, start, end);
	if(BFS_find(map, start) == 1)
	{
		print_map(map, "BFS");
	}	
	else
		cout<<"no way...\n";
	
	map_init(map, start, end);
	if(ASTAR_find(map, start ,end) == 1)
	{
		print_map(map, "A_STAR");
	}	
	else
		cout<<"no way...\n";
	

	for(int i=0; i<DIM; i++){
		delete[] pre_pos[i];
	}
	delete[] pre_pos;
	return 0;	
}












