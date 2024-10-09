#include <stdio.h> 
#include <string.h>
#include <math.h>
#include <unistd.h>

#define LENGHT         5
#define RADIUS         2
#define THETA_SPACING  0.02
#define ROTATION_ANGLE 0.03
#define CAM_DISTANCE   5
#define SCREEN_WIDHT   80
#define SCREEN_HEIGHT  24
#define SCALE          30

#define CylinderX(theta) RADIUS * cos(theta)
#define CylinderY(theta) RADIUS * sin(theta)

#define reset_cursor()    printf("\033[H")
#define hidden_cursor()   printf("\033[?25l")

float ld[] = {0, 1, -1};// light direction
const char lightPoint[] = ".,-~:;=!*#$@";

char buffer[1760];
float z_buffer[1760];

void render(float A){
    memset(buffer, ' ', sizeof(char) * 1760);
    memset(z_buffer, 0, sizeof(float) * 1760);

    for(float theta = 0; theta < 6.28; theta += THETA_SPACING){
        float xCylinder = CylinderX(theta);
        float yCylinder = CylinderY(theta);
        float RotateCos = cos(A);
        float RotateSin = sin(A);

        for(float l = (-LENGHT / 2.0); l < (LENGHT/2.0); l += 0.07){
            float x = xCylinder * RotateCos - l * RotateSin;
            float y = yCylinder; // ja que a altura nao se altera
            float z = xCylinder * RotateSin + l * RotateCos;

            // noção de profundidade, projecao
            float D = 1.0/(z + CAM_DISTANCE);
            float xp = (int)(SCREEN_WIDHT/2.0 + SCALE * D * x);
            float yp = (int)(SCREEN_HEIGHT/2.0 + SCALE/2.0 * D * y);

            // vetor normal
            float nx = cos(theta) * RotateCos;
            float ny = sin(theta);
            float nz = cos(theta) * RotateSin;

            // produto escalar com a direção para ver a intensidade
            float l_normal = sqrt(nx * nx + ny * ny + nz * nz); // normalizar o vertor
            float l_light = sqrt(ld[0]*ld[0] + ld[1]*ld[1] + ld[2] * ld[2]); // normaliza vetor luz
            float luminance = (nx * ld[0] + ny * ld[1] + nz * ld[2])/(l_normal * l_light);

            int idx = (int)((luminance + 1)*5.5); // move o intervalo de [-1, 1] para [0, 2]
                                                  // multiplica por 5.5 ja que temos 11 opções de iluminação
            
            int point = xp + SCREEN_WIDHT * yp;
            if(SCREEN_HEIGHT > yp && yp > 0 && SCREEN_WIDHT > xp && xp > 0 && D > z_buffer[point]){
                z_buffer[point] = D;
                buffer[point] = lightPoint[idx > 0 ? idx > 11 ? 11 : idx : 0];
            }     
        }
    }
}

int main(){
    float A = 0;
    hidden_cursor();
    while(1){
        reset_cursor();
        render(A);
        for(int i = 0; i<1761; i++){
            putchar(i % SCREEN_WIDHT ? buffer[i] : '\n');
        }
        A += ROTATION_ANGLE;
        usleep(30000);
    }
}
