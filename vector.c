#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define SCREEN_WIDTH  40
#define SCREEN_HEIGHT 20
#define BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define reset_cursor()    printf("\033[H")
#define hidden_cursor()   printf("\033[?25l")

#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)

#define norm(x, y) (sqrt((x) * (x) + (y) * (y)))

// um vetor 2D pode ser representado pela conexão entre 2 pontos em um plano cartesiano

char buffer[BUFFER_SIZE];

void pinta(float a, float b) {
    memset(buffer, ' ', BUFFER_SIZE);  // Limpa o buffer
    float t = 0;
    int normaab = (int)norm(a, b);

    while (1) {
        int xp = CENTER_X + (int)round(t * a);
        int yp = CENTER_Y - (int)round(t * b);

        if ((int)norm(t * a, t * b) > normaab || xp < 0 || xp >= SCREEN_WIDTH ||
            yp < 0 || yp >= SCREEN_HEIGHT) {
            break;
        }

        buffer[yp * SCREEN_WIDTH + xp] = '@';  // Desenha o ponto
        t += 0.1;
    }
}

/*
    Matriz de rotação 2d;
    |  cosA  senA |
    | -senA  cosA |

    dado um vetor v(a, b), v(a', b') seria

    |  cosA  senA |   |a|   a' = a.cosA + b.senA
    |             | . | | = 
    | -senA  cosA |   |b|   b' = -a.senA + b.cosA
*/


int main(){
    float A = 0.f;
    hidden_cursor();
    do {
        reset_cursor();
        int a = 5;
        int b = 5;
        pinta(a*cos(A) + b*sin(A), -a*sin(A) + b*cos(A));

        for(int i = 0; i<800; i++)
            putchar(i % SCREEN_WIDTH ? buffer[i] : '\n');
        usleep(10000);

    } while(A += 0.03);    

}