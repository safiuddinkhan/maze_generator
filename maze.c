#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>


#define left 1
#define right 2
#define top 3
#define bottom 4

#define high 1
#define low 0




struct cell{
int left_wall;
int right_wall;
int top_wall;
int bottom_wall;
int visited;
};



char corner[16][4] = {" ","╶","╴","─","╷","┌","┐","┬","╵","└","┘","┴","│","├","┤","┼"};
////////////////////////////////////////////////////////////////////////////////////



struct stack_cell{
int y;
int x;
struct stack_cell* back;
};


void stack_insert(struct stack_cell **curr,int y,int x){
struct stack_cell *prev_c;
if(*curr == NULL){
prev_c = NULL;
}else{
prev_c = *curr;
}

*curr = (struct stack_cell *)malloc(sizeof(struct stack_cell));
struct stack_cell *c = *curr;
c->y = y;
c->x = x;
c->back = prev_c;
}


void stack_drop(struct stack_cell **curr){
struct stack_cell *c = *curr;
struct stack_cell *prev = c->back;
*curr = prev;
free(c);
}
////////////////////////////////////////////////////////////////////////////////////
struct maze_context{
int w;
int h;
struct cell* c;
int visit_count;
};


void init_maze_context(struct maze_context* m,int h,int w){
m->visit_count = 0;
m->c = (struct cell*)malloc(sizeof(struct cell)*h*w);
m->w = w;
m->h = h;

struct cell* m1;

for(int y = 0;y < h;y++){
for(int x = 0;x < w;x++){
m1 = &m->c[(y*w)+x];
m1->left_wall = 1;
m1->right_wall = 1;
m1->top_wall = 1;
m1->bottom_wall = 1;
m1->visited = 0;
}}



}


void free_maze_context(struct maze_context* m){
free(m->c);
free(m);
}
//////////////////////////////////////////////////////////////////////////////////////

int get_leftwall_code(struct maze_context* maze,int y,int x){


struct cell* c = maze->c;
int w = maze->w;


struct cell* m;
struct cell* m1;
m = &c[(y*w)+x];
uint8_t l = 0;
uint8_t r = m->top_wall;
uint8_t t = 0;
uint8_t b = m->left_wall;

if(x != 0){
m1 = &c[(y*w)+(x-1)];
l = m1->top_wall;
}

if(y != 0){
m1 = &c[((y-1)*w)+x];
t = m1->left_wall;
}

//TBLR

int cc = r | l << 1 | b << 2 | t << 3;  
return cc;
}



int get_bottomwall_code(struct maze_context* maze,int y,int x){

struct cell* c = maze->c;
int w = maze->w;

struct cell* m;
struct cell* m1;
m = &c[(y*w)+x];
uint8_t l = 0;
uint8_t r = m->bottom_wall;
uint8_t t = m->left_wall;
uint8_t b = 0;

if(x != 0){
m1 = &c[(y*w)+(x-1)];
l = m1->bottom_wall;
}

//TBLR

int cc = r | l << 1 | b << 2 | t << 3;  
return cc;
}



int get_rightwall_code(struct maze_context* maze,int y,int x){
struct cell* c = maze->c;
int w = maze->w;

struct cell* m;
struct cell* m1;
m = &c[(y*w)+x];
uint8_t l = m->top_wall;
uint8_t r = 0;
uint8_t t = 0;
uint8_t b = m->right_wall;

if(y != 0){
m1 = &c[((y-1)*w)+x];
t = m1->right_wall;
}

//TBLR

int cc = r | l << 1 | b << 2 | t << 3;  
return cc;
}


int get_brcorner_code(struct maze_context* maze){

struct cell* c = maze->c;
int h = maze->h;
int w = maze->w;

struct cell* m;
struct cell* m1;
m = &c[((h-1)*w)+(w-1)];
uint8_t l = m->bottom_wall;
uint8_t r = 0;
uint8_t t = m->right_wall;
uint8_t b = 0;

//TBLR

int cc = r | l << 1 | b << 2 | t << 3;  
return cc;
}

//////////////////////////////////////////////////////////////

void draw_maze(struct maze_context* maze,int y_pos,int x_pos){
// \033[<y>;<x>H


int h = maze->h;
int w = maze->w;
struct cell* c = maze->c;


struct cell *m;
struct cell *m_top;
struct cell *m_bottom;
int x_gap = 4;
int y_gap = 2;

for(int y = 0;y < h;y++){
for(int x = 0;x < w;x++){
int cc = get_leftwall_code(maze,y,x);
printf("\033[%d;%dH%s",(y*y_gap)+y_pos,(x*x_gap)+x_pos,corner[cc]);
}}



for(int x = 0;x< w;x++){
int cc = get_bottomwall_code(maze,(h-1),x);
printf("\033[%d;%dH%s",((h-1)*y_gap)+y_pos+2,(x*x_gap)+x_pos,corner[cc]);
}


for(int y = 0;y < h;y++){
int cc = get_rightwall_code(maze,y,(w-1));
printf("\033[%d;%dH%s",(y*y_gap)+y_pos,((w-1)*x_gap)+x_pos+4,corner[cc]);
}



int cc1 = get_brcorner_code(maze);
printf("\033[%d;%dH%s",((h-1)*y_gap)+y_pos+2,((w-1)*x_gap)+x_pos+4,corner[cc1]);


for(int y = 0;y < h;y++){
for(int x = 0;x < w;x++){
m = &c[(y*w)+x];
//////
if(m->top_wall){
printf("\033[%d;%dH───",(y*y_gap)+y_pos,(x*x_gap)+x_pos+1);
}else{
printf("\033[%d;%dH   ",(y*y_gap)+y_pos,(x*x_gap)+x_pos+1);
}


if(m->bottom_wall){
printf("\033[%d;%dH───",(y*y_gap)+y_pos+2,(x*x_gap)+x_pos+1);
}else{
printf("\033[%d;%dH   ",(y*y_gap)+y_pos+2,(x*x_gap)+x_pos+1);
}


if(m->left_wall){
printf("\033[%d;%dH│",(y*y_gap)+y_pos+1,(x*x_gap)+x_pos);
}else{
printf("\033[%d;%dH ",(y*y_gap)+y_pos+1,(x*x_gap)+x_pos);
}


if(m->right_wall){
printf("\033[%d;%dH│",(y*y_gap)+y_pos+1,(x*x_gap)+x_pos+4);
}else{
printf("\033[%d;%dH ",(y*y_gap)+y_pos+1,(x*x_gap)+x_pos+4);
}
//////
}}

printf("\033[1B\n");
}

///////////////////////////////////////////////////////////////////////////


void toggle_visited(struct maze_context* maze,int y,int x,int t){

struct cell* c = maze->c;
int w = maze->w;

struct cell* m;
m = &c[(y*w)+x];
if(m->visited == 0 && t == high){
maze->visit_count++;
}


m->visited = t;
}






int is_wall_visited(struct maze_context* maze,int y,int x,int wall){


struct cell* c = maze->c;
int w = maze->w;
int h = maze->h;

struct cell* m;
struct cell* m1;

m = &c[(y*w)+x];
int left_v = 0;
int right_v = 0;
int top_v = 0;
int bottom_v = 0;

if(x == 0){
left_v = 1;
}else{
m1 = &c[(y*w)+(x-1)];
left_v = m1->visited;
}

if(x == w-1){
right_v = 1;
}else{
m1 = &c[(y*w)+(x+1)];
right_v = m1->visited;
}

if(y == 0){
top_v = 1;
}else{
m1 = &c[((y-1)*w)+x];
top_v = m1->visited;
}

if(y == h-1){
bottom_v = 1;
}else{
m1 = &c[((y+1)*w)+x];
bottom_v = m1->visited;
}


switch(wall){
case left:
return left_v;
case right:
return right_v;
case top:
return top_v;
case bottom:
return bottom_v;
}


}




int is_all_visited(struct maze_context* maze,int y,int x){

struct cell* c = maze->c;
int w = maze->w;
int h = maze->h;

struct cell* m;
struct cell* m1;

m = &c[(y*w)+x];
int left_v = 0;
int right_v = 0;
int top_v = 0;
int bottom_v = 0;

if(x == 0){
left_v = 1;
}else{
m1 = &c[(y*w)+(x-1)];
left_v = m1->visited;
}

if(x == w-1){
right_v = 1;
}else{
m1 = &c[(y*w)+(x+1)];
right_v = m1->visited;
}

if(y == 0){
top_v = 1;
}else{
m1 = &c[((y-1)*w)+x];
top_v = m1->visited;
}

if(y == h-1){
bottom_v = 1;
}else{
m1 = &c[((y+1)*w)+x];
bottom_v = m1->visited;
}

///////


if(left_v && right_v && top_v && bottom_v)
return 1;
else
return 0;
}
///////////////////////////////////////////////////////////////////////////


void toggle_wall(struct maze_context* maze,int y,int x,int wall,int t){

struct cell* c = maze->c;
int w = maze->w;
int h = maze->h;

struct cell* m;
struct cell* m1;
m = &c[(y*w)+x];

switch(wall){
case left:
if(x != 0){
m->left_wall = t;
m1 = &c[(y*w)+(x-1)];
m1->right_wall = t;
}else{
m->left_wall = t;
}

break;
case right:
if(x != w-1){
m->right_wall = t;
m1 = &c[(y*w)+(x+1)];
m1->left_wall = t;
}else{
m->right_wall = t;
}

break;
case top:
if(y != 0){
m->top_wall = t;
m1 = &c[((y-1)*w)+(x)];
m1->bottom_wall = t;
}else{
m->top_wall = t;
}

break;
case bottom:
if(y != h-1){
m->bottom_wall = t;
m1 = &c[((y+1)*w)+(x)];
m1->top_wall = t;
}else{
m->bottom_wall = t;
}

break;
}






}



void generate_maze(struct maze_context *maze){

if(maze->h <= 1 && maze->w <= 1){
return;
}

time_t t;
srand((unsigned) time(&t));
struct stack_cell *stack_curr = NULL;
int maze_h = maze->h;
int maze_w = maze->w;

int r_y = rand() % maze_h;
int r_x = rand() % maze_w;
int r_c = 0;
int back_y;
int back_x;
int back_c;


stack_insert(&stack_curr,r_y,r_x);


int k;
struct cell* m;



while(1){

///////////////////////////////////////////////////
toggle_visited(maze,r_y,r_x,high);
///////////////////////////////////////////////////

m = &maze->c[(r_y*maze_w)+r_x];

int cc;
int all;
while(1){
r_c = (rand() % 4)+1;
cc = is_wall_visited(maze,r_y,r_x,r_c);
all = is_all_visited(maze,r_y,r_x);
if(all)
break;

if(!cc)
break;
}
///////////////////////////////////////////////////



stack_insert(&stack_curr,r_y,r_x);



/////////////////////////////////
if(is_all_visited(maze,r_y,r_x)){



int cc;

while(1){
stack_drop(&stack_curr);

if(stack_curr == NULL)
break;

back_y = stack_curr->y;
back_x = stack_curr->x;
cc = is_all_visited(maze,back_y,back_x);




if(!cc)
break;

}

r_y = back_y;
r_x = back_x;

while(1){
r_c = (rand() % 4)+1;
cc = is_wall_visited(maze,r_y,r_x,r_c);
all = is_all_visited(maze,r_y,r_x);
if(all)
break;

if(!cc)
break;
}

}
/////////////////////////////////



toggle_wall(maze,r_y,r_x,r_c,low);
struct cell* m;
m = &maze->c[(r_y*maze_w)+r_x];

switch(r_c){
case left:
if(r_x != 0)
r_x = r_x - 1;
break;
case right:
if(r_x != maze_w-1)
r_x = r_x + 1;
break;
case top:
if(r_y != 0)
r_y = r_y -1;
break;
case bottom:
if(r_y != maze_h-1)
r_y = r_y + 1;
break;
}



if(maze->visit_count == (maze_h * maze_w)-1){
break;
}


}

// clear the stack
while(stack_curr != NULL){
stack_drop(&stack_curr);
}


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(){


int maze_h;
int maze_w;

time_t t;
srand((unsigned) time(&t));


maze_h = 15;
maze_w = 20;





struct maze_context *maze1 = (struct maze_context *)malloc(sizeof(struct maze_context));
init_maze_context(maze1,maze_h,maze_w);



printf("\033[2J");   // clear screen



generate_maze(maze1);
draw_maze(maze1,1,1);
free_maze_context(maze1);


return 0;
}