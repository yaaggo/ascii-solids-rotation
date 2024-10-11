#include <stdio.h> 
#include <string.h>
#include <math.h>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>

    void usleep(__int64 usec) {
        HANDLE timer;
        LARGE_INTEGER ft;

        ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

        timer = CreateWaitableTimer(NULL, TRUE, NULL);
        SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);
    }
#endif

#define LENGHT         5
#define RADIUS         2
#define THETA_SPACING  0.02
#define ROTATION_ANGLE 0.03
#define CAM_DISTANCE   5
#define SCREEN_WIDHT   160
#define SCREEN_HEIGHT  44
#define SCALE          30

#define BUFFER_SIZE    SCREEN_WIDHT *  SCREEN_HEIGHT

#define reset_cursor()    printf("\033[H")
#define hide_cursor()   printf("\033[?25l")
#define reveals_cursor()   printf("\033[?25h") // executar esse comando solto faz o cursor apareceu novamente

#define CylinderX(theta) RADIUS * cos(theta)
#define CylinderY(theta) RADIUS * sin(theta)

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

char lightPoint[] = ".,-~:;=!*#$@";
char buffer[BUFFER_SIZE];
float z_buffer[BUFFER_SIZE];
float ld[] = {0, 1, -1};// light direction

void render(float A1, float A2, float A3){
    memset(buffer, ' ', sizeof(char) * BUFFER_SIZE);
    memset(z_buffer, 0, sizeof(float) * BUFFER_SIZE);

    for(float theta = 0; theta < 6.28; theta += THETA_SPACING){
        float xCylinder = CylinderX(theta);
        float yCylinder = CylinderY(theta);

        for(float l = (-LENGHT / 2.0); l < (LENGHT/2.0); l += 0.07){
            float x = xCylinder;
            float y = yCylinder; 
            float z = l;

            // rotações
            ROTATE_X(x, y, z, A1);
            ROTATE_Y(x, y, z, A2);
            ROTATE_Z(x, y, z, A3);

            // Projeção
            float D = 1.0/(z + CAM_DISTANCE);
            float xp = (int)(SCREEN_WIDHT/2.0 + SCALE * D * x);
            float yp = (int)(SCREEN_HEIGHT/2.0 + SCALE/2.0 * D * y + 2);

            // Cálculo da normal
            float nx = xCylinder / RADIUS; // tem que ser um vetor unitario
            float ny = yCylinder / RADIUS;
            float nz = 0;  // Para o cilindro, a normal z não depende de l, apenas da superfície lateral

            // as mesmas rotações à normal
            ROTATE_X(nx, ny, nz, A1);
            ROTATE_Y(nx, ny, nz, A2);
            ROTATE_Z(nx, ny, nz, A3);

            // normalizaçao normal
            float l_normal = norm(nx, ny, nz);
            nx /= l_normal;
            ny /= l_normal;
            nz /= l_normal;

            // Iluminação
            float l_light = norm(ld[0], ld[1], ld[2]); 
            float luminance = (nx * ld[0] + ny * ld[1] + nz * ld[2])/(l_normal * l_light);

            int idx = (int)((luminance + 1)*5.5); // Mapeia a intensidade de iluminação

            int point = xp + SCREEN_WIDHT * yp;
            if(SCREEN_HEIGHT > yp && yp > 0 && SCREEN_WIDHT > xp && xp > 0 && D > z_buffer[point]){
                z_buffer[point] = D;
                buffer[point] = lightPoint[idx > 0 ? idx > 11 ? 11 : idx : 0];
            }     
        }
    }
}

int main(){
    float A1 = 0, A2 = 0, A3 = 0;
    hide_cursor();
    while(1){
        reset_cursor();
        render(A1, A2, A3);
        for(int i = 0; i<=BUFFER_SIZE; i++){
            putchar(i % SCREEN_WIDHT ? buffer[i] : '\n');
        }
        A1 += ROTATION_ANGLE;
        A2 += ROTATION_ANGLE / 2; // Diferentes velocidades para cada rotação
        A3 += ROTATION_ANGLE / 3;
        usleep(16000);
    }
}
