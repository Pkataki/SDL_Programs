//PAULO AUGUSTO_GOMES_KATAKI	
//complilaçao: g++ lab3.cpp -lSDL -w
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

const double EPS = 1e-9;
class color_rgb
{
    public : 
    int r,g,b;
    color_rgb(int r, int g, int b) : r(r), g(g), b(b) {}
};
class point 
{
    public : 
    int x;
    int y;
    //point(){}
    point(int x, int y) : x(x), y(y) {}
};

class line
{
    public : 
    point p_begin;
    point p_end;
//  line(){}
    line(point p_begin, point p_end) : p_begin(p_begin), p_end(p_end) {}
};
class polygon
{
    public : 
    color_rgb color;
    int num_lines;
    int num_points;
    bool have_to_color;
    vector< point > points;
    vector< line  > lines;
    polygon(int num_points, int num_lines, bool have_to_color, color_rgb color):
    num_points(num_points),
    num_lines(num_lines),
    have_to_color(have_to_color),
    color(color)
    {
        points.resize(num_points, point(0,0));
        lines.resize(num_lines, line(point(0,0),point(0,0)));
    }
    void create_point(int i, int x, int y)
    {
        points[i] = point(x,y);
    }
    void create_line(int i, int idx_p1, int idx_p2)
    {
        lines[i] = line(points[idx_p1], points[idx_p2]);
    }
    
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x *bpp;
   *(Uint32 *)p = pixel;
}
bool on_x_limit(int x)
{
    return (x >= 0 && x < SCREEN_WIDTH);
}
bool on_y_limit(int y)
{
    return (y >= 0 && y < SCREEN_HEIGHT);
}
void draw_pixel_screen(SDL_Surface * screen,int x, int y, color_rgb cor)
{
    if(!on_x_limit(x))
    	return;
	if(!on_y_limit(y))
   		return; 
    Uint32 pix;

    int r,g,b;
    pix = SDL_MapRGB(screen->format, cor.r, cor.g, cor.b);    

    putpixel(screen, x, y, pix);
}


void linhaDDA(SDL_Surface* screen, int xi, int yi, int xf, int yf, color_rgb cor)
{
    int dx = xf - xi, dy = yf - yi, steps;
    double incX, incY, x = xi, y = yi;

    steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    incX = dx/(double) steps;
    incY = dy/(double) steps;
    draw_pixel_screen(screen,x,y,cor);
 
    for(int i = 0; i < steps; i++)
    {
        x += incX;
        y += incY;
        draw_pixel_screen(screen,(int)x,(int)y,cor);
    }
}

bool are_equal(double x, double y)
{
    return (fabs(x - y) < EPS);
}

void desenha_poligono(SDL_Surface* screen , polygon p)
{
    int xi,yi,xf,yf;
    color_rgb black = color_rgb(0,0,0);
    int ymax = INT_MIN;
    int ymin = INT_MAX;
    for(int i = 0; i < p.num_lines; i++)
    {
        xi = p.lines[i].p_begin.x;
        yi = p.lines[i].p_begin.y;
        xf = p.lines[i].p_end.x;
        yf = p.lines[i].p_end.y;
        ymin = min(ymin,min(yi,yf));
        ymax = max(ymax,max(yi,yf));
    }

    if(p.have_to_color == false)
    {
        for(int i = 0; i < p.num_lines; i++)
        {
            xi = p.lines[i].p_begin.x;
            yi = p.lines[i].p_begin.y;
            xf = p.lines[i].p_end.x;
            yf = p.lines[i].p_end.y;
            linhaDDA(screen,xi,yi,xf,yf,black);
        }
        return;
    }
    vector< vector<double> >table;
    vector<double>table_row(4);
    for(int i = 0 ; i < p.num_lines; i++)
    {
        double y_min_line = min((double)p.lines[i].p_begin.y, (double)p.lines[i].p_end.y);
        
        double y_max_line = max((double)p.lines[i].p_begin.y, (double)p.lines[i].p_end.y);
        
       if(are_equal(y_min_line,y_max_line))
          continue;

        double deltax = ((double)p.lines[i].p_end.x - (double)p.lines[i].p_begin.x);

        double deltay = ((double)p.lines[i].p_end.y - (double)p.lines[i].p_begin.y);
        
        double inverse_m;

        if(are_equal(deltay,0))
            inverse_m = 0.0;
        else
            inverse_m = deltax/deltay;
        
        double x_y_min_line;

        if(are_equal(p.lines[i].p_begin.y,y_min_line))
            x_y_min_line = (double)(p.lines[i].p_begin.x);
        else
            x_y_min_line = (double)(p.lines[i].p_end.x);
        
        table_row[0] = y_min_line;
        table_row[1] = y_max_line;
        table_row[2] = x_y_min_line;
        table_row[3] = inverse_m; 
        table.push_back(table_row);
    }
    
   
    for(int y_var = ymin + 1; y_var < ymax; y_var++)
    {
        vector<pair<int,int> >x_intersect;

        for(int i = 0; i < table.size(); i++)
        {

            if(y_var > table[i][1] || y_var < table[i][0])
                continue;
            double x_var = table[i][3] * ((double)y_var - table[i][0]) + table[i][2];
            x_intersect.push_back(make_pair((int)x_var, i));
        }
        sort(x_intersect.begin(),x_intersect.end());

        for(int j = 0; j + 1 < x_intersect.size(); )
        {
            pair<int,int> x1 = x_intersect[j];

            pair<int,int> x2 = x_intersect[j+1];
        
            if(x1.first == x2.first && !are_equal(table[x1.second][0],table[x2.second][0]) && !are_equal(table[x1.second][1],table[x2.second][1]))
            {
                j++;
                continue;
            }
            
            j += 2;
            
            linhaDDA(screen, x1.first, y_var, x2.first, y_var, p.color);  
        }
    }

    for(int i = 0; i < p.num_lines; i++)
    {
        xi = p.lines[i].p_begin.x;
        yi = p.lines[i].p_begin.y;
        xf = p.lines[i].p_end.x;
        yf = p.lines[i].p_end.y;
        linhaDDA(screen,xi,yi,xf,yf,black);
    }
}
void test_polygon(SDL_Surface *screen,  SDL_Window *window)
{
    polygon p = polygon(8,8,1,color_rgb(100,160,100));
    
    p.create_point(0,0,10);
    
    p.create_point(1,40,30);
    
    p.create_point(2,20,50);
    
    p.create_point(3,60,70);
    
    p.create_point(4,110,30);
    
    p.create_point(5,130,0);

    p.create_point(6,100,30);

    p.create_point(7,80,0);
    
    for(int i = 0 ; i < p.num_points; i++)
    {
        p.create_line(i,i,(i+1)%p.num_points);
    }

    desenha_poligono(screen,p);
    SDL_UpdateWindowSurface(window); 
  
}
int main()
{
    SDL_Surface *screen;
    SDL_Window *window;
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO); 

   
    window = SDL_CreateWindow("TRABALHO 3 COMPUTACAO GRAFICA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    screen = SDL_GetWindowSurface(window);

    SDL_FillRect(screen, NULL, 0xFFFFFF); 

    SDL_Event event;

    test_polygon(screen,window);

    while(true)
	{
        if(SDL_PollEvent( &event) == 1)
        {
        	//cout << "teve um evento\n";
       		if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
            //	cout << "foi um esc\n";
             	break;
            }
            else if ( event.type == SDL_QUIT )
    				break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
