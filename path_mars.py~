import Image

green = (0,255,0);

img = Image.open("mars.png");
w,h = img.size;
pix = img.load();

start = (0,0);
end = (0,0);

costs = [];
marked = [];

home = (256,256)
cities = [	(123,456), #Hamsterdam
		(256,256), #Caterberg
		(30,30),   #New New York
		(400,345), #Cloudsdale
		(10,10),   #Umm Yeah
		(300,145), #Woosterborough
		(182,226), #Nyanilly
		(500,500), #Lord Conrad Lake City
		(350,350)  #Fork Bomb City
	]

print cities;

def main():
    global img;
    global costs;
    global start;
    global end;

    init_costs();
    init_marked();
    
    start = home;
    end = (182,226);

    print "Start: ",start;
    print "End: ",end;
    find_path(start,end);
    #show_marked();
    img.show();

def find_path(s,e):
    global costs;
    global marked;
    global pix;
    global img;

    sx,sy = s;
    ex,ey = e;

    marked[sx][sy] = True;
    cont=True;
    while cont:
        fringe = [];
        fringe = get_fringe();
	cont = process_fringe_element(fringe[0]);
	print len(fringe);
	#show_marked();
    traceback();

def traceback():
    global pix;
    global marked;
    global costs;
    global end;
    global start;
    global w;
    global h;

    x,y = end;

    while(not (x,y)==start):
        print (x,y);
        x,y=find_next_trace(x,y);
        #img.show();
        
    
def find_next_trace(x,y):
    global start;
    
    for i in range(3):
        for j in range(3):
            a,b = wrap(i+x-1,j+y-1);
            if(marked[a][b]==False):
                continue;
            if((a,b)==(x,y)):
                continue;
            cost1 = costs[x][y];
            cost2 = costs[a][b];
            diff=cost2-cost1;
	    comp = diff-cost((x,y),(a,b));
	    price = cost((x,y),(a,b));
            print start,end,(x,y),(a,b),cost1,cost2,diff,comp;
            if((a,b)==start):
                pix[x,y] = green;
                return (a,b);
            if(comp < 0.001 and comp > -0.001):
                pix[x,y] = green;
                return (a,b);
    
        
    
def process_fringe_element(element):
    global marked;
    global costs;
    global pix;
    global end;
    
    val,x,y = element;

    flag=True;

    for i in range(3):
        for j in range(3):
            a,b = wrap(i+x-1,j+y-1);
            if((a,b)==end):
                costs[a][b] = cost((x,y),(a,b));
                marked[a][b] = True;
                flag = False;
            elif((a,b)==(x,y)):
                continue;
            elif(marked[a][b]):
                continue;
            else:
                #print (a,b),(x,y),(costs[a][b]),(costs[x][y]),(cost((x,y),(a,b)));
                costs[a][b] = cost((x,y),(a,b));
                marked[a][b] = True;
    return flag;
    
def show_marked():
    global marked;
    global pix;
    global w;
    global h;
    global img;

    for i in range(w):
        for j in range(h):
            if(marked[i][j]):
                pix[i,j] = (0,255,0);

def get_fringe():
    global marked;
    global w;
    global h;

    fringe = []

    for i in range(w):
        for j in range(h):
            if(on_edge(i,j)):
                fringe.append((costs[i][j],i,j));
    fringe.sort();
    return fringe;
    
def on_edge(x,y):
    global marked;

    if(marked[x][y]==False):
        return False;
    
    marked_count = 1
    unmarked_count = 0;
    for i in range(3):
        for j in range(3):
            a,b=wrap(i+x-1,j+y-1)
    	    if(marked[a][b]==True):
		    marked_count += 1;
	    elif(marked[a][b]==False):
		    unmarked_count += 1;
	    else:
		    print "Strange markings on the wall..";
    if(marked_count > 0 and unmarked_count > 0):
	    return True;
    return False;

def wrap(x,y):
    global w;
    global h;
	
    x = x%w;
    y = y%h;
    return x,y

def init_costs():
    global costs;
    global w;
    global h;
    for i in range(w):
        costs.append([]);
        for j in range(h):
            costs[i].append(0);

            
def init_marked():
    global marked;
    global w;
    global h;
    for i in range(w):
        marked.append([]);
        for j in range(h):
            marked[i].append(0);


def zero_array(array):
    global w;
    global h;
    for i in range(w):
        for j in range(h):
            array[i][j] = 0;

def populate_costs():
    global w;
    global h;
    global costs;
    global pix;
    
    for i in range(w):
        for j in range(h):
            if(pix[i,j]==black):
                costs[i][j] = -1;
            elif(pix[i,j]==white):
                costs[i][j] = 1;
            elif(pix[i,j]==blue):
                costs[i][j] = 2;
            else:
                costs[i][j] = 1;
                
                
    
def cost(s,e):
    global costs;
    global wall_cost;

    sx,sy = s;
    ex,ey = e;


    d = pow((ex-sx)**2+(ey-sy)**2,0.5)
    h = abs(pix[ex,ey][1]-pix[sx,sy][1]);

    return (d+20*h+costs[ex][ey]);

def find_s_e():
    global start;
    global end;
    global w;
    global h;
    global pix;
    
    for i in range(w):
        for j in range(h):
            if(pix[i,j]==green):
                start = (i,j);
            elif(pix[i,j]==red):
                end = (i,j);






main();
