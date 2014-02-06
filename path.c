#include "path.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <Imlib2.h>
#include <pthread.h>
#include <sys/resource.h>


#define MIN(X,Y)	((X < Y) ? (X) : (Y))
#define MAX(X,Y)	((X > Y) ? (X) : (Y))

#define NORM	0
#define START	1
#define END	2
#define WATER	3
#define WALL	4
#define TAGGED	5
#define PATH	6


int main(int argc, char **argv){
	img_t *img;
	img = ld_img_img(argv[1]);
	set_vals(img);
	find_path(img);
	mk_img_img(img,argv[2]);
	return 0;
}

void find_path(img_t *img){
	int sx,sy;
	int ex,ey;
	node *current;
	node *fringe;
	int **mat;
	img_t *cp;
	int notdone;

	cp_img(&cp,img);

	sx = img->sx;
	sy = img->sy;
	ex = img->ex;
	ey = img->ey;

	mat = img->matrix;

	fringe = NULL;

	mat[sx][sy] = TAGGED;

	notdone=1;

	while(notdone){
		free_fringe(fringe);
		fringe = get_fringe(img);
		current = fringe;
		do{
			if(current->x==ex && current->y ==ey){
				backtrack(img,cp);
				notdone=0;
				break;	
			}else{
				set_surroundings(img,current->x,current->y);
				current = current->next;
			}
		}while(current->next != NULL);
		mk_img_img(img,"test.png");

	}



}

void backtrack(img_t *img,img_t *orig){
	int x,y;
	int bt;
	x = img->ex;
	y = img->ey;

	while(x != img->sx || y != img->sy){
		printf("%d %d\n",x,y);
		bt = get_backtrack(orig,img,x,y);
		switch(bt){
			case 1:
				x -= 1;
				y -= 1;
				break;
			case 2:
				y -= 1;
				break;
			case 3:
				x+=1;
				y-=1;
				break;
			case 4:
				x-=1;
				break;
			case 5:
				x+=1;
				break;
			case 6:
				x-=1;
				y+=1;
				break;
			case 7:
				y+=1;
				break;
			case 8:
				x+=1;
				y+=1;
				break;
			default:
				fprintf(stderr,"What kind of direction is that?: %d\n",bt);
return;
		}
		x = wrap(x,img->width);
		y = wrap(y,img->height);
		img->matrix[x][y] = PATH;	
	}
}
int get_backtrack(img_t *orig,img_t *img, int x, int y){
	int i,j;
	int w,h;

	w = img->width;
	h = img->height;

	x = wrap(x,img->width);
	y = wrap(y,img->height);

	i = wrap(x-1,w);
	j = wrap(y-1,h);
	
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 1;
	}

	i = wrap(x,w);
	j = wrap(y-1,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 2;
	}
	
	i = wrap(x+1,w);
	j = wrap(y-1,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 3;
	}
	
	i = wrap(x-1,w);
	j = wrap(y,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 4;
	}
	
	i = wrap(x+1,w);
	j = wrap(y,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 5;
	}
	
	i = wrap(x-1,w);
	j = wrap(y+1,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 6;
	}
	
	i = wrap(x,w);
	j = wrap(y+1,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 7;
	}
	
	i = wrap(x+1,w);
	j = wrap(y+1,h);
	if((img->vals[i][j] - img->vals[x][y]) < 3){
		return 8;
	}
	return 0;
}


void free_fringe(node *head){
	node *next;
	node *current;
	current = head;
	if(current==NULL){
		return;
	}
	while(current->next != NULL){
		next = current->next;
		free(current);
		current = next;
	}
	free(current);

}

node *get_fringe(img_t *img){
	int i,j;
	int w,h;
	node *new;
	node *current;

	w = img->width;
	h = img->height;

	new = newnode(0,0,0);
	current = new;

	for(i=0; i<w; i++){
		for(j=0; j<h; j++){
			if(on_edge(img,i,j)){
				current->next = newnode(i,j,img->vals[i][j]);
				current->next->prev = current;
				current = current->next;
			}
		}
	}

	return new->next;

	
}

int on_edge(img_t *img,int x, int y){
	int i,j;

	x -= 1;
	y -= 1;

	for(i=0; i < 3; i++){
		for(j=0; j<3; j++){
			if(i==1 && j==1){
				continue;
			}
			if(get_val(img,x+i,y+j)!=TAGGED){
				return 1;
			}
			
		}
	}
	return 0;
	
}

void set_vals(img_t *img){
	int i,j;
	int val;

	for(i=0; i < img->width; i++){
		for(j=0; j<img->height; j++){
			val = img->matrix[i][j];
			switch(val){
				case NORM:
					img->vals[i][j] = 1;
					break;
				case START:
					img->vals[i][j] = 0;
					break;
				case END:
					img->vals[i][j] = 1;
					break;
				case WATER:
					img->vals[i][j] = 2;
					break;
				case WALL:
					img->vals[i][j] = 10000;
					break;
				default:
					fprintf(stderr,"Unknown type at %d %d.\n",i,j);
					return;
		
			}	
		}
	}
}

void set_surroundings(img_t *img, int x, int y){
	int i,j;

	x -= 1;
	y -= 1;

	for(i=0; i < 3; i++){
		for(j=0; j<3; j++){
			if(i==1 && j==1){
				continue;
			}else if(get_val(img,x+i,y+j)==TAGGED){
				continue;
			}
			set_val(img,TAGGED,x+i,y+j);
			set_dist(img,x+1,y+1,x+i,y+j);
		}
	}
	
}

void set_dist(img_t *img,int sx, int sy, int ex, int ey){
	sx = wrap(sx,img->width);
	sy = wrap(sy,img->height);
	ex = wrap(ex,img->width);
	ey = wrap(ey,img->height);
	
	img->vals[ex][ey]=img->vals[ex][ey]+img->vals[sx][sy];
	
}


int count_near(img_t *img, int x,int y){
	int i,j;
	int count;

	x -= 1;
	y -= 1;

	count = 0;

	for(i=0; i < 3; i++){
		for(j=0; j<3; j++){
			if(i==1 && j==1){
				continue;
			}
			if(get_val(img,i+x,j+y)==TAGGED){
				count++;
			}	
		}
	}

	return count;
}

node *newnode(int x, int y,float val){
	node *new;
	new = calloc(1,sizeof(node));
	new->x = x;
	new->y = y;
	new->val = val;
	return new;
}

void app_node(node *head,node *new){
	node *current;
	current = head;
	if(head==NULL){
		fprintf(stderr,"Tried to append to a null head\n");
		return;
	}
	while(current->next != NULL){

		current = current->next;
	}
	current->next = new;
	new->prev = current;
}

node *lowest(node *head){
	node *current;
	node *lowest;

	lowest = head;
	current = head;
	if(head==NULL){
		fprintf(stderr,"Tried to append to a null head\n");
		return NULL;
	}
	while(current->next != NULL){

		current = current->next;
		if(current->val < lowest->val){
			lowest = current;
		}
	}
	return lowest;
}

void set_stack_size(int mb){
	const rlim_t kStackSize = mb * 1024 * 1024;   // increase the minimum stack size. 
	struct rlimit rl;
	int result;
	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0)
	{
	        if (rl.rlim_cur < kStackSize)
	        {
	                rl.rlim_cur = kStackSize;
		        result = setrlimit(RLIMIT_STACK, &rl);
		        if (result != 0)
		        {
		                   fprintf(stderr, "setrlimit returned result = %d\n", result);
		        }
		}
	}
}


void cp_img(img_t **dest, img_t *src){
	int w,h;
	int i,j;
	
	w = src->width;
	h = src->height;

	*dest = new_img(w,h);

	(*dest)->width = w;
	(*dest)->height = h;
	(*dest)->sx = src->sx;
	(*dest)->sy = src->sy;
	(*dest)->ex = src->ex;
	(*dest)->ey = src->ey;


	for(i=0; i < w; i++){
		for(j=0; j < h; j++){
			(*dest)->matrix[i][j] = src->matrix[i][j];
		}
	}

	for(i=0; i<w; i++){
		for(j=0; j<h; j++){
			(*dest)->vals[i][j] = src->vals[i][j];
		}
	}

}

/**
 * This function takes a file name and attempts to load 
 * that file as a img and return a pointer to said img.
 * @param name	This is the name of the file to be loaded.
 * @return 	Returns a pointer to the loaded img.
 */
img_t *ld_img_img(const char *name){
	int w,h;
	int i,j;
	Imlib_Color col;

	img_t *newimg;

	Imlib_Image image;
	printf("Loading Image: %s\n",name);
	image = imlib_load_image(name);
	imlib_context_set_image(image);

	w = imlib_image_get_width();
	h = imlib_image_get_height();


	newimg = new_img(w,h);

	for(i=0; i<w; i++){
		for(j=0; j<h; j++){
			imlib_image_query_pixel(i,j,&col);
			if(col.green == 255 && col.red == 0 && col.blue==0 ){
				newimg->matrix[i][j] = START;
				newimg->sx = i;
				newimg->sy = j;
			}else if(col.red==255 && col.green==0 && col.blue==0){
				newimg->matrix[i][j] = END;
				newimg->ex = i;
				newimg->ey = j;
			}else if(col.red ==0 && col.green == 0 && col.blue==0){
				newimg->matrix[i][j] = WALL;
			}else if(col.red == 0 && col.green==0 && col.blue==255){
				newimg->matrix[i][j] = WATER;
			}else if(col.red==255 && col.green==255 && col.blue==255){
				newimg->matrix[i][j] = NORM;
			}
		}
	}
	
	return newimg;


}
/**
 * This function takes in a pointer to a img and outputs the
 * img as a .png. The file name is in the format
 *  <Cost>_<x>_<y>_<h>.png
 *  @param	img	the img to output as an image
 */
void mk_img_img(img_t *img,const char *name){
	int i,j;
	Imlib_Image image;
	int w,h;
	int **mat;
	int val;
	int sx,sy;
	int ex,ey;

	sx = img->sx;
	sy = img->sy;
	ex = img->ex;
	ey = img->ey;


	mat = img->matrix;

	w = img->width;
	h = img->height;
	
	if(sx == 0 && sy == 0){
		sx = 0;
		sy = 1;
		ex = w-1;
		ey = h-2;
	}

	image = imlib_create_image(w,h);

	imlib_context_set_image(image);

	for(i=0; i < w; i++){
		for(j=0; j < h; j++){
			val = mat[i][j];
			if(i==sx && j==sy){
				imlib_context_set_color(0,255,0,255);
			}else if(i==ex && j == ey){
				imlib_context_set_color(255,0,0,255);
			}else if(img->matrix[i][j] == PATH){
				imlib_context_set_color(0,0,255,255);		
			}else{
				imlib_context_set_color(val,val,val,255);
			}
			imlib_image_draw_pixel(i,j,0);
		}
	}
	
	imlib_save_image(name);
	imlib_free_image();

}

/**
 * wraps a variable to the other side of a img. This way
 * the img loops.
 * @param	x	variable to wrap
 * @param	side	length of a side of the img
 * @return	wrapped variable
 */
inline int wrap(register int x, register int side){
	
	if(x%side==0)return 0;
	if(x < 0){ 
		x = side+(x%side);
	}else{
		x = x % side;
	}
	return x;
	
}
/**
 * gets a value on the img using the wrap function for safety
 * @param	img	img to get from
 * @param	x	x position
 * @param	y	y position
 * @return	value of the img at (x,y)
 */

inline int get_val(img_t *img, int x, int y){
	
	register int w,h;

	
	w = img->width;
	h = img->height;
	
//	x = wrap(x,w);
//	y = wrap(y,h);

	if(x < 0) return PATH;
	if(x >= w) return PATH;
	if(y < 0) return PATH;
	if(y >= h) return PATH;
	
	return img->matrix[x][y];

}
/**
 * sets a value on a img using the wrap function for safety
 * @param	img	img to set value on
 * @param	x	x position
 * @param	y	y position
 */
inline void set_val(img_t *img, int val, int x, int y){


	register int w,h;

	
	w = img->width;
	h = img->height;

	x = wrap(x,w);
	y = wrap(y,h);

	img->matrix[x][y] = val;
	

}
/**
 * frees a img.
 * @param	img	img to free
 */
void free_img(img_t *img){
	int i;
	int **mat;

	mat = img->matrix;
	
	for(i=0; i < img->width; i++){
		free(mat[i]);
	}
	free(mat);
	free(img);

}


/**
 * callocs a new img.
 * @param	side	length of a side of the img.
 * @return	a pointer to the new img.
 */
img_t *new_img(int w, int h){
	
	//printf("Making new img struct.\n");
	int i;
	img_t *newimg;
	newimg = calloc(1,sizeof(img_t));
	newimg->matrix = calloc(w,sizeof(int*));
	for(i=0;i<w;i++){
		newimg->matrix[i] = calloc(h,sizeof(int));
	}
	newimg->width = w;
	newimg->height = h;
	newimg->vals = calloc(w,sizeof(float*));
	for(i=0; i<w; i++){
		newimg->vals[i] = calloc(h,sizeof(float));
	}

	return newimg;

}




