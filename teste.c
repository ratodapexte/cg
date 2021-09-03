#include "GL/gl.h"
#include "GL/glut.h"
#include "stdlib.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

typedef struct Vertex
{
    float x, y, z; // posicao do vertice
    float r, g, b; // cor do vertice
} Vertex;

Vertex *newVertex(float x, float y, float z, float r, float g, float b)
{
    Vertex *vertex = malloc(sizeof(Vertex *));
    vertex->x = x;
    vertex->y = y;
    vertex->z = z;
    vertex->r = r;
    vertex->g = g;
    vertex->b = b;

    return vertex;
}

// Vertex *vertexes[] = {newVertex(-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f),
//                       newVertex(0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
//                       newVertex(0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f)};

Vertex vertexes[] = {
    {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f};

// void display()
// {
//     glutSwapBuffers();
// }
// Variaveis para controlar a camera
float fov_y;
float cam_x, cam_y, cam_z;
float center_x, center_y, center_z;

// Variaveis para posicionar a parede
float parede_x, parede_y, parede_z;                    // posicao da parede
float parede_rotacao;                                  // rotacao da parede
float parede_largura, parede_altura, parede_espessura; //dimensoes da parede
    // ...

    void
    display()
{
    int i;
    Vertex v;
    // ...
    for (i = 0; i < sizeof(vertexes); i++)
    {
        v = vertexes[i];
        glColor3f(v.r, v.g, v.b);
        glVertex3f(v.x, v.y, v.z);
    }
    
    // ...
    // Seleciona a matriz ModelView e reseta todas as transformacoes
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Define a matriz View
    gluLookAt(cam_x, cam_y, cam_z, center_x, center_y, center_z, 0.0f, 1.0f, 0.0f);
    // Define a matriz Model
    /*
    * Lembre -se , a ordem das transformacoes eh o inverso de
    * como aparecem no codigo , por isso aplicamos a escala ,
    * depois as rotacoes e finalmente a translacao
    */
    glTranslatef(parede_x, parede_y, parede_z);
    glRotatef(parede_rotacao, 0.0f, 1.0f, 0.0f);            // rotacao em torno do eixo y
    glScalef(parede_largura, parede_altura, parede_espessura); // como o cubo tem lado 1 podemos escalar ele para as dimensoes desejadas
    /*
    * Desenha o cubo de lado 1 com todas as transformacoes
    acima
    * aplicadas .
    * A origem do cubo eh no centro , entao lembre de colocar
    * altura /2 na coordenada y da posicao para ficar
    * perfeitamente no chao
    */
    glutSolidCube(1.0f);
    // ...
}

int main(int argc, char **argv)
{
    // Inicializacao
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("OpenGL");

    // Registra o callback de renderizacao (por enquanto nao faz nada)
    glutDisplayFunc(display);

    // limpa a tela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // sinaliza para o OpenGL que queremos desenhar triangulos
    glBegin(GL_TRIANGLES);

    display();
    fov_y = 75.0f; // 75 graus
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov_y, 1.0f * WINDOW_WIDTH / WINDOW_HEIGHT,
                   0.001f, 1000.0f);
    // sinaliza para o OpenGL que terminamos o desenho do objeto
    glEnd();

    // Troca os buffers
    glutSwapBuffers();

    // Inicia o loop de eventos da GLUT
    glutMainLoop();
    return 0;
}