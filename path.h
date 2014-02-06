#include <pthread.h>

struct img_t{
	int width, height;
	int **matrix;
	int sx,sy;
	int ex,ey;
	float **vals;
};
typedef struct img_t img_t;

struct node{
	int x,y;
	float val;
	struct node *prev;
	struct node *next;
};
typedef struct node node;



img_t *new_img(int w,int h);

void set_val(img_t *img, int val, int x, int y);

int get_val(img_t *img, int x, int y);
void free_img(img_t *img);

img_t *ld_img_img(const char *name);
void mk_img_img(img_t *img, const char *name);
inline int wrap(register int x, register int side);
void set_stack_size(int mb);
void cp_img(img_t **dest, img_t *src);
int count_near(img_t *img, int x,int y);
void find_path(img_t *img);
void backtrack(img_t *img,img_t *orig);
int get_backtrack(img_t *orig,img_t *img, int x, int y);
void free_fringe(node *head);
node *get_fringe(img_t *img);
int on_edge(img_t *img,int x, int y);
void set_vals(img_t *img);
void set_surroundings(img_t *img, int x, int y);
void set_dist(img_t *img,int sx, int sy, int ex, int ey);
int count_near(img_t *img, int x,int y);
node *newnode(int x, int y,float val);
void app_node(node *head,node *new);
node *lowest(node *head);
float get_cost(int type);
