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
	return 0;
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
	return newimg;

}




