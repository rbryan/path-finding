import Image

black=(0,0,0);
white=(255,255,255);
red=(255,0,0);
green=(0,255,0);
blue=(0,0,255);

img = Image.open("path_finding.png");
w,h = img.size;
pix = img.load();

start = (0,0);
end = (0,0);

costs = [];
marked = [];



def main():
    global img;
    global costs;
    
    init_costs();
    init_marked();
    populate_costs();
    
    find_s_e();
    
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

    sx,sy = start;
    ex,ey = end;

    marked[sx][sy] = True;
    cont=True;
    while cont:
        fringe = [];
        fringe = get_fringe();
        for i in fringe:
            if(process_fringe_element(i)==False):
                cont = False;  
                break;
        print len(fringe);
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
            cost = costs[x][y];
            cost2 = costs[a][b];
            diff=cost-cost2;
            print start,end,(x,y),(a,b),cost,cost2,diff;
            if((a,b)==start):
                pix[x,y] = green;
                return (a,b);
            if(diff==2 and pix[x,y]==blue):
                pix[x,y] = green;
                return (a,b);
            if(diff==1 and (pix[x,y]==red or pix[x,y]==green or pix[x,y]==white)):
                pix[x,y] = green;
                return (a,b);
            if(diff==-1 and pix[x,y]==black):
                pix[x,y] = green;
                return  (a,b);
    
        
    
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
            if(pix[a,b]==black):
                continue;
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
    
    found_marked=False;
    for i in range(3):
        for j in range(3):
            a,b=wrap(i+x-1,j+y-1)
            if(marked[a][b]==True and found_marked==False):
                found_marked=True;
            if(found_marked==True and marked[a][b]==False):
                return True;
    return False;

def wrap(x,y):
    global w;
    global h;

    if(x%w==0):
        x = 0;
    if(x < 0):
        x = w+(x%w);
    else:
        x = x%w;

    if(y%w==0):
        y = 0;
    if(y < 0):
        y = h+(y%h);
    else:
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

    return (costs[sx][sy]+costs[ex][ey]);

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
