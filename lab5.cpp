 //PAULO AUGUSTO_GOMES_KATAKI	
//complilaçao: g++ lab5.cpp -lSDL2 -w
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

const int INIT_POSX = 300;
const int INIT_SCALEX = 50;
const int INIT_POSY = 300;
const int INIT_SCALEY = 50;
const int INIT_POSZ = 300;
const int INIT_SCALEZ = 50;

const int FACTOR_ROTATION = 5;
const int FACTOR_SCALE = 5;
const int FACTOR_TRANSLATE = 5;

const double EPS = 1e-9;
class color_rgb
{
    public : 
    int r,g,b;
    color_rgb(int r, int g, int b) : r(r), g(g), b(b) {}
    color_rgb & operator= (const color_rgb & other)
    {
        if (this != &other) 
        {
            r = other.r;
            g = other.g;
            b = other.b;
        }
        return *this;
    }
};
class point 
{
    public : 
    double x;
    double y;
    double z;
    double m;
    point(){}
    point(double x, double y, double z, double m) : x(x), y(y), z(z), m(m) {}
    point & operator= (const point & other)
    {
        if (this != &other) 
        {
             this->x = other.x;
             this->y = other.y;
             this->z = other.z;
             this->m = other.m;
        }
        return *this;
    }
};

class line
{
    public : 
    int idx1;
    int idx2;
  	line(){}
    line(int idx1, int idx2) : idx1(idx1), idx2(idx2) {}
    line & operator= (const line & other)
    {
        if (this != &other) 
        {
             this->idx1 = other.idx1;
             this->idx2 = other.idx2;
        }
        return *this;
    }
};
class polygon
{
    public : 
        color_rgb color;
        int m;
        int num_lines;
        int num_points;
        bool have_to_color;
        double rx,ry,rz,sx,sy,sz,tx,ty,tz;
        vector< point > points;
        vector< line  > lines;
        polygon(int num_points, int num_lines, bool have_to_color, color_rgb color, int m):
        num_points(num_points),
        num_lines(num_lines),
        rx(0),
        ry(0),
        rz(0),
        ty(0),
        tx(0),
        tz(0),
        sy(0),
        sx(0),
        sz(0),
        have_to_color(have_to_color),
        m(m),
        color(color)
        {
            points.resize(num_points, point(0,0,0,1));
            
        }
        void create_point(int i, double x, double y, double z)
        {
            points[i] = point(x,y,z,m);
        }
        void create_line(int idx_p1, int idx_p2)
        {
            lines.push_back(line(idx_p1, idx_p2));
        }
        void clear_operations()
        {
        	rx = 0;
        	ry = 0;
        	rz = 0;
        	ty = 0;
        	tx = 0;
        	tz = 0;
        	sy = 0;
        	sx = 0;
        	sz = 0;
        }
        polygon & operator= (const polygon & other)
        {
            if (this != &other)
            {
                 this->m = other.m;
                 this->num_lines = other.num_lines;
                 this->num_points = other.num_points;
                 this->have_to_color = other.have_to_color;
                 this->points = other.points;
            }
             return *this;
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

point transform(point p, double mat[][4])
{
    vector<double> v(4);
    v[0] = p.x;
    v[1] = p.y;
    v[2] = p.z;
    v[3] = p.m;

    vector<double>v1(4,0);
    cout << "matriz = \n";
    for(int j = 0; j < 4; j++)
    {
        
        for(int k = 0; k < 4; k++)
        {
            cout << mat[j][k] << ' ';
        }
        cout << endl;
    }
    for(int j = 0; j < v.size(); j++)
    {
        double aux = 0.0;
        for(int k = 0; k < v.size(); k++)
        {
            aux += (v[k]*mat[k][j]);
        }
        v1[j] = aux;
    }
    cout << "val de v3 " << v[3] << endl;
    return point(v1[0]/v1[3], v1[1]/v1[3], v1[2]/v1[3], v1[3]);
}

point project(int t, point a)
{
	//cavaleira
    if(t == 0)
    {
   		
        double k = 0.5;
        double ang = (45.0*M_PI/180.0);
        double cav[][4] = {{1.0,           0.0,              0.0, 0.0}, 
                            {0.0,           1.0,             0.0, 0.0}, 
                            {-k*cos(ang), -k*sin(ang), 1.0, 0.0}, 
                            {0.0,           0.0,             0.0, 1.0}};
    
        return transform(a,cav);
    }
    //axonometrica isometrica
    else if(t == 1)
    {
    	
        double ang1 = (45.0*M_PI/180.0);
        double ang2 = (30.0*M_PI/180.0);
        double axo[][4] = {{cos(ang1),     sin(ang1)*cos(ang2), 0.0, 0.0}, 
                            {0.0,           cos(ang2),          0.0, 0.0}, 
                            {sin(ang1), -cos(ang1)*sin(ang2),   0.0, 0.0}, 
                            {0.0,           0.0,                0.0, 1.0}};
    
        return transform(a,axo);
    }
    //cabinet
    else if(t == 2)
    {
        double ang1 = (25.0*M_PI/180.0);
        
        double cab[][4] = { {1.0,           0.0,           0.0 , 0.0}, 
                            {0.0,           1.0,           0.0 , 0.0}, 
                            {0.5*cos(ang1), 0.5*sin(ang1), 0.0,  0.0}, 
                            {0.0,           0.0,           0.0,  1.0}};
    
        return transform(a,cab);
    }
    //1 ponto de fuga
    else if(t == 3)
    {
        double fz = -1000;
        double fk = 300;
        double fj = 300;
        double mat1[][4] = {  {1.0,    0.0,    0.0,  0.0}, 
                              {0.0,    1.0,    0.0,  0.0}, 
                              {0.0,    0.0,    1.0,  0.0}, 
                              {-fk,    -fj,    0.0,  1.0}};
        
        double mat2[][4] = {  {1.0,    0.0,    0.0,  0.0}, 
                              {0.0,    1.0,    0.0,  0.0}, 
                              {0.0,    0.0,    1.0,  0.0}, 
                              {fk,    fj,    0.0,  1.0}};

        double onevp[][4] = { {1.0,    0.0,    0.0,  0.0}, 
                              {0.0,    1.0,    0.0,  0.0}, 
                              {0.0,    0.0,    1.0,  -1.0/fz}, 
                              {0.0,    0.0,    0.0,  1.0}};
    	a = transform(a,mat1);
        a = transform(a,onevp);
        return transform(a,mat2);
    }
    //2 pontos de fuga
    else if(t == 4)
    {
        double fz = -1000;
        double fx = -1000;
       	double fk = 500;
        double fj = 500;
        double mat1[][4] = {  {1.0,    0.0,    0.0,  0.0}, 
                              {0.0,    1.0,    0.0,  0.0}, 
                              {0.0,    0.0,    1.0,  0.0}, 
                              {0.0,    -fj,    0.0,  1.0}};
        
        double mat2[][4] = {  {1.0,    0.0,    0.0,  0.0}, 
                              {0.0,    1.0,    0.0,  0.0}, 
                              {0.0,    0.0,    1.0,  0.0}, 
                              {0,       fj,    0.0,  1.0}};

        double twovp[][4] = { {1.0,    0.0,    0.0,  -1.0/fx}, 
                              {0.0,    1.0,    0.0,  0.0}, 
                              {0.0,    0.0,    1.0,  -1.0/fz}, 
                              {0.0,    0.0,    0.0,  1.0}};
    
        a = transform(a,mat1);
        a = transform(a,twovp);
        return transform(a,mat2);
    }
}

point translate(point p , double tx, double ty, double tz)
{
        
    double transla[][4] = { {1.0, 0.0, 0.0, 0.0}, 
                            {0.0, 1.0, 0.0, 0.0}, 
                            {0.0, 0.0, 1.0, 0.0},  
                            {tx,  ty,  tz,  1.0}};
        
    return transform(p,transla);
}
//falta o Z
point scale(point p, double sx, double sy, double sz)
{

       //cout << xl << ' ' << yl << endl;
    double escal[][4] = {{sx, 0.0, 0.0, 0.0}, 
                        {0.0, sy,  0.0, 0.0}, 
                        {0.0, 0.0, sz,  0.0},  
                        {0.0, 0.0, 0.0, 1.0}};
    
    return transform(p,escal);
}

point rotate(point p, double rx, double ry, double rz)
{
        double ax = rx*M_PI/180.0; 
        double ay = ry*M_PI/180.0; 
		double az = rz*M_PI/180.0; 	

        double rotz[][4] = {{cos(az),  sin(az), 0.0, 0.0}, 
                            {-sin(az), cos(az), 0.0, 0.0}, 
                            {0.0,      0.0,   1.0, 0.0}, 
                            {0.0,      0.0,   0.0, 1.0}};
        
        double rotx[][4] = {{1.0,    0.0,    0.0,    0.0}, 
                            {0.0,  cos(ax), sin(ax), 0.0}, 
                            {0.0, -sin(ax), cos(ax), 0.0},  
                            {0.0,    0.0,    0.0,    1.0}};
        
        double roty[][4] = {{cos(ay), 0.0, -sin(ay), 0.0},
                            {0.0,    1.0,  0.0,    0.0}, 
                            {sin(ay), 0.0, cos(ay),  0.0} , 
                            {0.0,    0.0,  0.0,    1.0}};

        p = transform(p,rotz);
        p = transform(p,roty);
        p = transform(p,rotx);
        return p;              
}

void draw_polygon(SDL_Surface *screen, polygon p, int type_projection)
{
    int xi, xf,yi,yf;
    point p1,p2;
    for(int i = 0; i < p.num_lines; i++)
    {
        p1 = project(type_projection,translate(scale(rotate(p.points[p.lines[i].idx1],p.rx, p.ry , p.rz ), INIT_SCALEX+p.sx, INIT_SCALEY+p.sy, INIT_SCALEZ+p.sz),INIT_POSX+p.tx, INIT_POSY+p.ty, INIT_POSZ+p.tz));
        xi = p1.x;
        yi = p1.y;
        p2 = project(type_projection,translate(scale(rotate(p.points[p.lines[i].idx2],p.rx, p.ry , p.rz ), INIT_SCALEX+p.sx, INIT_SCALEY+p.sy, INIT_SCALEZ+p.sz),INIT_POSX+p.tx, INIT_POSY+p.ty, INIT_POSZ+p.tz));
        xf = p2.x;
        yf = p2.y;
        linhaDDA(screen,xi,SCREEN_HEIGHT - yi,xf, SCREEN_HEIGHT - yf,color_rgb(100,100,150));
    }
}

void test_polygon(SDL_Surface *screen,  SDL_Window *window)
{
    polygon p = polygon(8,12,0,color_rgb(100,160,100),1);
    
    p.create_point(0,1,1,-1);
    p.create_point(1,1,1,1);
    
    p.create_point(2,1,-1,-1);
    p.create_point(3,1,-1,1);
    
    p.create_point(4,-1,1,-1);
    p.create_point(5,-1,1,1);
    
    p.create_point(6,-1,-1,-1);
    p.create_point(7,-1,-1,1);
    
    p.create_line(0,1);
    p.create_line(0,4);
    p.create_line(0,2);
    
    p.create_line(1,5);
    p.create_line(1,3);

    p.create_line(2,6);
    p.create_line(2,3);
    
    p.create_line(3,7);
    p.create_line(4,5);
    
    p.create_line(4,6);
    p.create_line(5,7);
    
    p.create_line(6,7);
    

    int xi, xf,yi,yf;
    SDL_Event event;
    int type_projection = 4;
    while(true)
    {
        if(SDL_PollEvent( &event) == 1)
        {
            //cout << "teve um evento\n";
            if( event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    
                    //ROTAÇÃO ANGULOS POSITIVOS
                    case SDLK_q: //
                        p.rx += FACTOR_ROTATION;
                        break;
                    case SDLK_w: 
                        p.ry += FACTOR_ROTATION;
                        break;
                    case SDLK_e: 
                        p.rz += FACTOR_ROTATION;
                        break;

                    //ROTAÇÃO ANGULOS NEGATIVOS
                    case SDLK_a: // 
                        p.rx -= FACTOR_ROTATION;
                        break;
                    case SDLK_s:// 
                        p.ry -= FACTOR_ROTATION;
                        break;
                    case SDLK_d: // 
                        p.rz -= FACTOR_ROTATION;
                        break;


                    //TRANSLAÇÃO POSITIVA
                    case SDLK_r:
                    	p.tx += FACTOR_TRANSLATE;
                        break;
                    case SDLK_t: // 
                       	p.ty += FACTOR_TRANSLATE;
                        break;
                    case SDLK_y: // 
                        p.tz += FACTOR_TRANSLATE;
                        break;


                    //TRANSLAÇÃO NEGATIVA            
                    case SDLK_f: // 
                       	p.tx -= FACTOR_TRANSLATE;
                        break;
                    case SDLK_g: // 
                        p.ty -= FACTOR_TRANSLATE;
                        break;
                    case SDLK_h: // 
                       	p.tz -= FACTOR_TRANSLATE;
                        break;


                    //ESCALA POSITIVA 
                    case SDLK_u: // 
                       	p.sx += FACTOR_SCALE;
                        break;
                    case SDLK_i: // 
                       	p.sy += FACTOR_SCALE;
                        break;
                    case SDLK_o: // 
                        p.sz += FACTOR_SCALE;
                        break;

                    //ESCALA NEGATIVA 
                    case SDLK_j: 
                       	p.sx -= FACTOR_SCALE;
                        break;
                    case SDLK_k: 
                      	p.sy -= FACTOR_SCALE;
                        break;
                    case SDLK_l: 
                      	p.sz -= FACTOR_SCALE;
                        break;
                    case SDLK_1: 
                      	type_projection = 0;
                        break;
                    case SDLK_2: 
                      	type_projection = 1;
                        break;
                    case SDLK_3: 
                      	type_projection = 2;
                        break;
                    case SDLK_4: 
                      	type_projection = 3;
                        break;
                    case SDLK_5: 
                      	type_projection = 4;
                        break;    
                    case SDLK_SPACE: 
                      	p.clear_operations();
                        break;
                    case SDLK_F1:
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                        "Menu de opcoes",
                        "Q - Rotacao angulo positivo em x\nW - Rotacao angulo positivo em y\nE - Rotacao angulo positivo em z\nA - Rotacao angulo negativo em x\nS - Rotacao angulo negativo em y\nD - Rotacao angulo negativo em z\nR - Translada no eixo x com fator positivo\nT - Translada no eixo y com fator positivo\nY - Translada no eixo z com fator positivo\nF - Translada no eixo x com fator negativo\nG - Translada no eixo y com fator negativo\nH - Translada no eixo z com fator negativo\nU - Escala a coordenada x com fator positivo\nI - Escala a coordenada y com fator positivo\nO - Escala a coordenada z com fator positivo\nJ - Escala a coordenada x com fator negativo\nK - Escala a coordenada y com fator negativo\nL - Escala a coordenada z com fator negativo\nEspaço - Coloca o poligono nas coordenadas originais\n1 - Projeção Cavaleira\n2 - Projeção Isometrica\n3 - Projeção Cabinet\n4 - Projeção com um ponto de fuga\n5 - Projeção com 2 pontos de fuga\n",
                         NULL);
                        break;
                    case SDLK_ESCAPE:
                        break;
                }
               
            }
            else if ( event.type == SDL_QUIT )
                break;
            SDL_GetKeyboardState(NULL);
            SDL_FillRect(screen, NULL, 0xFFFFFF); 
            draw_polygon(screen,p,type_projection);
            
            SDL_UpdateWindowSurface(window);
        }
        
    }
  
}
int main()
{
    SDL_Surface *screen;
    SDL_Window *window;
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO); 

   	
    window = SDL_CreateWindow("TRABALHO 5 COMPUTACAO GRAFICA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    screen = SDL_GetWindowSurface(window);

    SDL_FillRect(screen, NULL, 0xFFFFFF); 
    SDL_UpdateWindowSurface(window); 
    

    test_polygon(screen,window);

    

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
