//PAULO AUGUSTO_GOMES_KATAKI	
//complilaçao: g++ lab2.cpp -lSDL -w
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
struct color
{
    int r,g,b;
    color(int r, int g, int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
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
    return (x > 0 && x < SCREEN_WIDTH);
}
bool on_y_limit(int y)
{
    return (y > 0 && y < SCREEN_HEIGHT);
}
void draw_pixel_screen(SDL_Surface * screen,int x, int y, color cor)
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


void linhaDDA(SDL_Surface* screen, int xi, int yi, int xf, int yf, color cor)
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

void circBrasenham(SDL_Surface* screen, int xc, int yc, int r, color cor)
{
    int x = 0;
    int y = -r;
    int RR = r*r;

    while(x < r)
    {
    
		draw_pixel_screen(screen,xc-x,yc+y,cor);
		draw_pixel_screen(screen, xc+x, yc-y-1,cor);
		draw_pixel_screen(screen,xc+x, yc+y,cor);
		draw_pixel_screen(screen,xc-x, yc-y-1,cor);

        int mh = ((x+1)*(x+1)) + (y*y) - RR; 
        int delta_i = ((x+1)*(x+1)) + ((y+1)*(y+1)) - RR;
        int mv = (x*x) + ((y+1)*(y+1)) - RR;
         
        // diagonal
        if(delta_i == 0)
        {
            x++;
            y++;
        }

        if(delta_i < 0)
        {
            int rho = mh - delta_i;

          // da frente
        if(rho > 0)
        {
            x++;
            continue;
        }

          // diagonal
            x++;
            y++;
        }

        if(delta_i > 0)
        {
            int rho = delta_i - mv;

          // B
        if(rho <= 0)
        {
            x++;
            y++;
            continue;
        }

          // C      
          y++;
        }
    }
}

int main()
{
    SDL_Surface *screen;
    SDL_Window *window;
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO); 

   
    window = SDL_CreateWindow("TRABALHO 2 COMPUTACAO GRAFICA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    screen = SDL_GetWindowSurface(window);

    SDL_Event event;

    while(true)
	{

        if(SDL_PollEvent( &event) == 1)
        {
        	cout << "teve um evento\n";
       		if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
            	cout << "foi um esc\n";
             	break;
            }
            else if ( event.type == SDL_QUIT )
    				break;
        }
        int xi = rand()%SCREEN_WIDTH;
        int yi = rand()%SCREEN_HEIGHT;
        int xf = rand()%SCREEN_WIDTH;
        int yf = rand()%SCREEN_HEIGHT;
        int ra = rand()%SCREEN_WIDTH;
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;
        int op = rand()%2;
        if(op == 0)
            linhaDDA(screen,xi,yi,xf,yf,color(r,g,b));
        else
            circBrasenham(screen,xi,yi,ra,color(r,g,b));
        SDL_UpdateWindowSurface(window); 
      	SDL_Delay(20);  
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
