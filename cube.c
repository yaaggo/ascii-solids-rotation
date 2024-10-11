#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>

    void usleep(__int64 usec) {
        HANDLE timer;
        LARGE_INTEGER ft;

        ft.QuadPart = -(10 * usec); // convertendo para o intervalo de  100 nanosegundos, valor negativo indica tempo relativo

        timer = CreateWaitableTimer(NULL, TRUE, NULL);
        SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);
    }
#endif

#define SCALE          40
#define CUBE_SIZE      20
#define CAM_DISTANCE   60
#define SCREEN_WIDHT   160
#define SCREEN_HEIGHT  44
#define ROTATION_ANGLE 0.03f
#define SPEED          0.6f

#define BUFFER_SIZE    SCREEN_WIDHT *  SCREEN_HEIGHT

#define reset_cursor()    printf("\033[H")
#define hide_cursor()   printf("\033[?25l")
#define reveals_cursor()   printf("\033[?25h")

#define norm(x, y, z) (sqrt((x) * (x) + (y) * (y) + (z) * (z)))

#define ROTATE_X(i, j, k, A) do {            \
    float temp_j = (j) * cos(A) - (k) * sin(A);  \
    float temp_k = (j) * sin(A) + (k) * cos(A);  \
    (j) = temp_j;                              \
    (k) = temp_k;                              \
} while(0)

#define ROTATE_Y(i, j, k, A) do {            \
    float temp_i = (i) * cos(A) + (k) * sin(A);  \
    float temp_k = -(i) * sin(A) + (k) * cos(A); \
    (i) = temp_i;                              \
    (k) = temp_k;                              \
} while(0)

#define ROTATE_Z(i, j, k, A) do {            \
    float temp_i = (i) * cos(A) - (j) * sin(A);  \
    float temp_j = (i) * sin(A) + (j) * cos(A);  \
    (i) = temp_i;                              \
    (j) = temp_j;                              \
} while(0)

const char lightPoint[] = ".,-~:;=!*#$@";
char    buffer[BUFFER_SIZE];
float z_buffer[BUFFER_SIZE];

float ld[3] = {-1, -1, -1};// light direction
float A1 = 0, A2 = 0, A3 = 0;

void surface(float x, float y, float z, int n){
    ROTATE_X(x, y, z, A1);
    ROTATE_Z(x, y, z, A2);
    ROTATE_Z(x, y, z, A3);

    float D = 1.0 / (z + CAM_DISTANCE);
    float xp = (int)(SCREEN_WIDHT/2.0 + SCALE * D * x); // se quiser colocar um deslocamento horizontal, so somar um valor a mais aqui
    float yp = (int)(SCREEN_HEIGHT/2.0 + SCALE/2.0 * D * y); 

    float nx = (x / CUBE_SIZE);
    float ny = (y / CUBE_SIZE);
    float nz = (z / CUBE_SIZE);

    // normalizar a normal dinâmica
    float l_normal = norm(nx, ny, nz);
    nx /= l_normal;
    ny /= l_normal;
    nz /= l_normal;

    /*r_xaxis(&nx, &ny, &nz, A1);
    r_yaxis(&nx, &ny, &nz, A2); 
    r_zaxis(&nx, &ny, &nz, A3);*/

    float l_light = norm(ld[0], ld[1], ld[2]);
    
    float luminance = (nx * ld[0] + ny * ld[1] + nz * ld[2])/(l_normal * l_light);

    int i_light = (int)((luminance + 1)*5.5);

    int idx = xp + SCREEN_WIDHT * yp;

    if(SCREEN_HEIGHT > yp && yp > 0 && SCREEN_WIDHT > xp && xp > 0 && D > z_buffer[idx]){
        buffer[idx] = lightPoint[i_light > 0 ? i_light > 11 ? 11 : i_light : 0];
        z_buffer[idx] = D;    
    }

}

void render(){
    memset(buffer, ' ', sizeof(char) * BUFFER_SIZE);
    memset(z_buffer, 0, sizeof(float) * BUFFER_SIZE);

    for(float xc = -CUBE_SIZE; xc < CUBE_SIZE; xc += SPEED){
        for(float yc = -CUBE_SIZE; yc < CUBE_SIZE; yc += SPEED){
            surface(xc, yc, -CUBE_SIZE, 0); // n = 0 para a frente
            surface(xc, yc, CUBE_SIZE, 1); // n = 1 para trás
            surface(CUBE_SIZE, yc, xc, 2); // n = 2 para a direita
            surface(-CUBE_SIZE, yc, xc, 3); // n = 3 para a esquerda
            surface(xc, CUBE_SIZE, yc, 4); // n = 4 para cima
            surface(xc, -CUBE_SIZE, yc, 5); // n = 5 para baixo
        }
    }
}

int main(){
    hide_cursor();
    while(1){
        reset_cursor();
        render();
        for(int i = 0; i<BUFFER_SIZE; i++)
            putchar(i % SCREEN_WIDHT ? buffer[i] : '\n');
        A1 += 0.05;
        A2 += 0.05;
        A3 += 0.01;
        usleep(16000);
    }
}