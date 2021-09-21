/*-------------------------------------------*/
/*------------ STANDARD INCLUDES ------------*/
/*-------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/*------------------------------------------*/
/*--------------- DATA TYPES ---------------*/
/*------------------------------------------*/
typedef struct Vec2 {
    float x, y;
} Vec2;

typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Transform {
    Vec3 position;
    Vec3 rotation;
} Transform;


/*-----------------------------------------*/
/*--------------- CONSTANTS ---------------*/
/*-----------------------------------------*/
const float DEG2RAD = 3.14159265 / 180.0f;


/*------------------------------------------*/
/*------------ GLOBAL VARIABLES ------------*/
/*------------------------------------------*/
Vec2 WINDOW_SIZE = {1280, 960};
Vec2 WINDOW_CENTER = {640, 480};
float FOVY = 75.0f;
float ZNEAR = 10e-3;
float ZFAR = 10e3;
Transform CAM;
int KEYBOARD[128] = {0};
Vec2 MOTION;


/*-------------------------------------------*/
/*-------------- OBJ MESH DATA --------------*/
/*-------------------------------------------*/
#define MAX_VERTICES 60000

// Object 1
unsigned int doors_tex_id;
int main_door_angle = 0;
int open_main_door = -1;
int DOOR_VERTEX_COUNT;
Vec3 DOOR_VERTICES[MAX_VERTICES];
Vec3 DOOR_NORMALS[MAX_VERTICES];
Vec2 DOOR_TEX_COORDS[MAX_VERTICES];

// Object 2
int side_door_angle = 0;
int open_side_door = -1;
int SECONDARY_DOOR_VERTEX_COUNT;
Vec3 SECONDARY_DOOR_VERTICES[MAX_VERTICES];
Vec3 SECONDARY_DOOR_NORMALS[MAX_VERTICES];
Vec2 SECONDARY_DOOR_TEX_COORDS[MAX_VERTICES];

// Object 3
unsigned int chair_tex_id;
int CHAIR_VERTEX_COUNT;
Vec3 CHAIR_VERTICES[MAX_VERTICES];
Vec3 CHAIR_NORMALS[MAX_VERTICES];
Vec2 CHAIR_TEX_COORDS[MAX_VERTICES];

// Object 4
unsigned int windows_tex_id;
int open_windows = -1;
int window_angle = 0;
int LEFT_WINDOW_VERTEX_COUNT;
Vec3 LEFT_WINDOW_VERTICES[MAX_VERTICES];
Vec3 LEFT_WINDOW_NORMALS[MAX_VERTICES];
Vec2 LEFT_WINDOW_TEX_COORDS[MAX_VERTICES];

// Object 5
int RIGHT_WINDOW_VERTEX_COUNT;
Vec3 RIGHT_WINDOW_VERTICES[MAX_VERTICES];
Vec3 RIGHT_WINDOW_NORMALS[MAX_VERTICES];
Vec2 RIGHT_WINDOW_TEX_COORDS[MAX_VERTICES];

// Object 6
unsigned int floor_tex_id;
int FLOOR_VERTEX_COUNT;
Vec3 FLOOR_VERTICES[MAX_VERTICES];
Vec3 FLOOR_NORMALS[MAX_VERTICES];
Vec2 FLOOR_TEX_COORDS[MAX_VERTICES];

// Object 7
unsigned int mesa_tex_id;
int MESA_VERTEX_COUNT;
Vec3 MESA_VERTICES[MAX_VERTICES];
Vec3 MESA_NORMALS[MAX_VERTICES];
Vec2 MESA_TEX_COORDS[MAX_VERTICES];

// Object 8
unsigned int freezer_tex_id;
int FREEZER_VERTEX_COUNT;
Vec3 FREEZER_VERTICES[MAX_VERTICES];
Vec3 FREEZER_NORMALS[MAX_VERTICES];
Vec2 FREEZER_TEX_COORDS[MAX_VERTICES];

// Object 9
unsigned int mug_tex_id;
int MUG_VERTEX_COUNT;
Vec3 MUG_VERTICES[MAX_VERTICES];
Vec3 MUG_NORMALS[MAX_VERTICES];
Vec2 MUG_TEX_COORDS[MAX_VERTICES];

// Object 10
unsigned int plate_tex_id;
int PLATE_VERTEX_COUNT;
Vec3 PLATE_VERTICES[MAX_VERTICES];
Vec3 PLATE_NORMALS[MAX_VERTICES];
Vec2 PLATE_TEX_COORDS[MAX_VERTICES];

// Object 11
unsigned int fan_tex_id;
int fan_speed = 1;
int FAN_VERTEX_COUNT;
Vec3 FAN_VERTICES[MAX_VERTICES];
Vec3 FAN_NORMALS[MAX_VERTICES];
Vec2 FAN_TEX_COORDS[MAX_VERTICES];

// Object 12
unsigned int painting_tex_id;
int PAINTING_VERTEX_COUNT;
Vec3 PAINTING_VERTICES[MAX_VERTICES];
Vec3 PAINTING_NORMALS[MAX_VERTICES];
Vec2 PAINTING_TEX_COORDS[MAX_VERTICES];

// Object 13
unsigned int walls_tex_id;
int WALLS_VERTEX_COUNT;
Vec3 WALLS_VERTICES[MAX_VERTICES];
Vec3 WALLS_NORMALS[MAX_VERTICES];
Vec2 WALLS_TEX_COORDS[MAX_VERTICES];

/*---------------------------------------------*/
/*-------------- SETUP FUNCTIONS --------------*/
/*---------------------------------------------*/
void init_gl();
void setup_lighting();
int load_obj(const char* path, int object);
int load_all_objects();
unsigned int load_texture(const char* path);
void load_all_textures();


/*---------------------------------------------*/
/*------------- CALLBACK FUNCIONS -------------*/
/*---------------------------------------------*/
void display();
void idle();
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);
void reshape(int width, int height);


/*---------------------------------------------*/
/*------------- DRAWING FUNCTIONS -------------*/
/*---------------------------------------------*/
void draw_grid(int n);
void main_door();
void side_door();
void windows();
void move_fan();


/*--------------------------------------------*/
/*-------------- MATH FUNCTIONS --------------*/
/*--------------------------------------------*/
Vec3 forward(Transform* t);
Vec3 right(Transform* t);
Vec3 up(Transform* t);


/*-------------------------------------------*/
/*-------------- MAIN FUNCTION --------------*/
/*-------------------------------------------*/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // Setting the window size, display mode, window name and placing the mouse pointer to the center of the screen
    glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Room");
    glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);

    // Setting the callback functions
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutReshapeFunc(reshape);

    // Initializing some options of OpenGL and lighting
    init_gl();

    // Setting initial camera position outside the room and facing the main door
    CAM.position = (Vec3) {2.0f, 4.0f, -15.0f};
    CAM.rotation = (Vec3) {0.0f, 90.0f, 0.0f};

    // Loading the object files
    load_all_objects();

	load_all_textures();

    // Initializing main loop
    glutMainLoop();

    return 0;
}

/*-----------------------------------------------------------*/
/*--------------------- SETUP FUNCTIONS ---------------------*/
/*-----------------------------------------------------------*/
void init_gl()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glutSetCursor(GLUT_CURSOR_NONE);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    setup_lighting();
}

void setup_lighting()
{
    float mat_shininess[] = {80.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    float light_diffuse[] = {1.0f, 1.0f, 1.0f};
    float light_position[] = {2.0f, 4.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    float light_specular[] = {0.0f, 1.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    float light_spot_direction[] = {2.17f, 7.24f, 109.0f};
    float light_spot_diffuse[] = {255.0f, 255.0f, 0.0f};
    float light_spot_position[] = {4.0763f, 1.0055f, 5.9039f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, light_spot_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_spot_diffuse);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_spot_direction);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

int load_obj(const char* path, int object)
{
    FILE *fp = fopen(path, "r");

    if(!fp) return 0;

    char buffer[512] = "";
    int vertex_count = 0;
    int normal_count = 0;
    int tex_coord_count = 0;

    Vec3 vertices[MAX_VERTICES];
    Vec3 normals[MAX_VERTICES];
    Vec2 tex_coords[MAX_VERTICES];

    while(fgets(buffer, 512, fp))
    {
        // Comment
        if(buffer[0] == '#') continue;

        char* token = strtok(buffer, " ");

        if(strcmp(token, "v") == 0)
        {
            // Parse vertex
            vertices[vertex_count].x = atof(strtok(NULL, " "));
            vertices[vertex_count].y = atof(strtok(NULL, " "));
            vertices[vertex_count].z = atof(strtok(NULL, " "));
            vertex_count++;
        }
        else if(strcmp(token, "vn") == 0)
        {
            // Parse normal vector
            normals[normal_count].x = atof(strtok(NULL, " "));
            normals[normal_count].y = atof(strtok(NULL, " "));
            normals[normal_count].z = atof(strtok(NULL, " "));
            normal_count++;
        }
        else if(strcmp(token, "vt") == 0)
        {
            // Parse texture coordinate
            tex_coords[tex_coord_count].x = atof(strtok(NULL, " "));
            tex_coords[tex_coord_count].y = -atof(strtok(NULL, " "));
            tex_coord_count++;
        }
        else if(strcmp(token, "f") == 0)
        {
            // Parse face
            int i;
            for(i = 0; i < 3; i++)
            {
                if(object == 1) // Main door
                {
                    DOOR_VERTICES[DOOR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    DOOR_TEX_COORDS[DOOR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    DOOR_NORMALS[DOOR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    DOOR_VERTEX_COUNT++;
                }
                else if(object == 2) // Secondary door
                {
                    SECONDARY_DOOR_VERTICES[SECONDARY_DOOR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    SECONDARY_DOOR_TEX_COORDS[SECONDARY_DOOR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    SECONDARY_DOOR_NORMALS[SECONDARY_DOOR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    SECONDARY_DOOR_VERTEX_COUNT++;
                }
                else if(object == 3) // Mesa
                {
                    CHAIR_VERTICES[CHAIR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    CHAIR_TEX_COORDS[CHAIR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    CHAIR_NORMALS[CHAIR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    CHAIR_VERTEX_COUNT++;
                }
                else if(object == 4) // Left window
                {
                    LEFT_WINDOW_VERTICES[LEFT_WINDOW_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    LEFT_WINDOW_TEX_COORDS[LEFT_WINDOW_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    LEFT_WINDOW_NORMALS[LEFT_WINDOW_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    LEFT_WINDOW_VERTEX_COUNT++;
                }
                else if(object == 5) // Right window
                {
                    RIGHT_WINDOW_VERTICES[RIGHT_WINDOW_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    RIGHT_WINDOW_TEX_COORDS[RIGHT_WINDOW_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    RIGHT_WINDOW_NORMALS[RIGHT_WINDOW_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    RIGHT_WINDOW_VERTEX_COUNT++;
                }
                else if(object == 6) // Chão
                {
                    FLOOR_VERTICES[FLOOR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    FLOOR_TEX_COORDS[FLOOR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    FLOOR_NORMALS[FLOOR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    FLOOR_VERTEX_COUNT++;
                }
                else if(object == 7) // Mesa
                {
                    MESA_VERTICES[MESA_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    MESA_TEX_COORDS[MESA_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    MESA_NORMALS[MESA_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    MESA_VERTEX_COUNT++;
                }else if(object == 8) // Geladeira
                {
                    FREEZER_VERTICES[FREEZER_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    FREEZER_TEX_COORDS[FREEZER_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    FREEZER_NORMALS[FREEZER_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    FREEZER_VERTEX_COUNT++;
                }else if(object == 9) // Caneca
                {
                    MUG_VERTICES[MUG_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    MUG_TEX_COORDS[MUG_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    MUG_NORMALS[MUG_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    MUG_VERTEX_COUNT++;
                }else if(object == 10) // Prato
                {
                    PLATE_VERTICES[PLATE_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    PLATE_TEX_COORDS[PLATE_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    PLATE_NORMALS[PLATE_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    PLATE_VERTEX_COUNT++;
                }else if(object == 11) // Ventilador
                {
                    FAN_VERTICES[FAN_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    FAN_TEX_COORDS[FAN_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    FAN_NORMALS[FAN_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    FAN_VERTEX_COUNT++;
                }
                else if(object == 12) // Quadro
                {
                    PAINTING_VERTICES[PAINTING_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    PAINTING_TEX_COORDS[PAINTING_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    PAINTING_NORMALS[PAINTING_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    PAINTING_VERTEX_COUNT++;
                }
                else if(object == 13) // Paredes
                {
                    WALLS_VERTICES[WALLS_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    WALLS_TEX_COORDS[WALLS_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    WALLS_NORMALS[WALLS_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    WALLS_VERTEX_COUNT++;
                }
            }
        }
    }

    fclose(fp);

    return 1;
}

int load_all_objects()
{
    if(!load_obj("objects/portaPrincipal.obj", 1))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/portaSecundaria.obj", 2))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/cadeira.obj", 3))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/Janela1.obj", 4))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/Janela2.obj", 5))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/chao.obj", 6))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/mesa.obj", 7))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/geladeira.obj", 8))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/caneca.obj", 9))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/prato.obj", 10))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/paletaVentilador.obj", 11))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/Quadro.obj", 12))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    if(!load_obj("objects/paredes.obj", 13))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
}

unsigned int load_texture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if(data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        perror("Texture failed to load");
        stbi_image_free(data);
    }
    return textureID;
}

void load_all_textures()
{
	doors_tex_id = load_texture("textures/portas.jpg");
    windows_tex_id = load_texture("textures/janela.jpg");
    mesa_tex_id = load_texture("textures/biro.jpg");
    chair_tex_id = load_texture("textures/cadeira.jpg");
    freezer_tex_id = load_texture("textures/freezer.jpg");    
    painting_tex_id = load_texture("textures/bandeira.png");
    fan_tex_id = load_texture("textures/ventilador.jpg");
    floor_tex_id = load_texture("textures/chao.jpg");
    walls_tex_id = load_texture("textures/quarto.jpg");

}


/*-----------------------------------------------------------*/
/*-------------------- CALLBACK FUNCIONS --------------------*/
/*-----------------------------------------------------------*/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // View matrix
    Vec3 fwd = forward(&CAM);
    Vec3 u = up(&CAM);
    Vec3 eye = CAM.position;
    Vec3 center = {eye.x + fwd.x, eye.y + fwd.y, eye.z + fwd.z};
    setup_lighting();

    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, u.x, u.y, u.z);

    draw_grid(50);

    int i;
    // Draw the main door
    glBindTexture(GL_TEXTURE_2D, doors_tex_id);
    glPushMatrix();
    glTranslatef(-0.192099, -0.052135, -8.332047);
    glRotatef(main_door_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.192099, 0.052135, 8.332047);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < DOOR_VERTEX_COUNT; i++)
    {
        glNormal3f(DOOR_NORMALS[i].x, DOOR_NORMALS[i].y, DOOR_NORMALS[i].z);
        glTexCoord2f(DOOR_TEX_COORDS[i].x, DOOR_TEX_COORDS[i].y);
        glVertex3f(DOOR_VERTICES[i].x, DOOR_VERTICES[i].y, DOOR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the side door
    glPushMatrix();
    glTranslatef(-1.743257, -0.122533, 7.634487);
    glRotatef(side_door_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(1.743257, 0.122533, -7.634487);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < SECONDARY_DOOR_VERTEX_COUNT; i++)
    {
        glNormal3f(SECONDARY_DOOR_NORMALS[i].x, SECONDARY_DOOR_NORMALS[i].y, SECONDARY_DOOR_NORMALS[i].z);
        glTexCoord2f(SECONDARY_DOOR_TEX_COORDS[i].x, SECONDARY_DOOR_TEX_COORDS[i].y);
        glVertex3f(SECONDARY_DOOR_VERTICES[i].x, SECONDARY_DOOR_VERTICES[i].y, SECONDARY_DOOR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw the left window
    glBindTexture(GL_TEXTURE_2D, windows_tex_id);
    glPushMatrix();
    glTranslatef(14.902783, 3.648618, -1.792326);
    glRotatef(window_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(-14.902783, -3.648618, 1.792326);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < LEFT_WINDOW_VERTEX_COUNT; i++)
    {
        glNormal3f(LEFT_WINDOW_NORMALS[i].x, LEFT_WINDOW_NORMALS[i].y, LEFT_WINDOW_NORMALS[i].z);
        glTexCoord2f(LEFT_WINDOW_TEX_COORDS[i].x, LEFT_WINDOW_TEX_COORDS[i].y);
        glVertex3f(LEFT_WINDOW_VERTICES[i].x, LEFT_WINDOW_VERTICES[i].y, LEFT_WINDOW_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the right window
    glPushMatrix();
    glTranslatef(14.903056, 3.648618, 3.201038);
    glRotatef(-window_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(-14.903056, -3.648618, -3.201038);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < RIGHT_WINDOW_VERTEX_COUNT; i++)
    {
        glNormal3f(RIGHT_WINDOW_NORMALS[i].x, RIGHT_WINDOW_NORMALS[i].y, RIGHT_WINDOW_NORMALS[i].z);
        glTexCoord2f(RIGHT_WINDOW_TEX_COORDS[i].x, RIGHT_WINDOW_TEX_COORDS[i].y);
        glVertex3f(RIGHT_WINDOW_VERTICES[i].x, RIGHT_WINDOW_VERTICES[i].y, RIGHT_WINDOW_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Draw the chão
    glBindTexture(GL_TEXTURE_2D, floor_tex_id);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    for(i = 0; i < FLOOR_VERTEX_COUNT; i++)
    {
        glNormal3f(FLOOR_NORMALS[i].x , FLOOR_NORMALS[i].y, FLOOR_NORMALS[i].z);
        glTexCoord2f(FLOOR_TEX_COORDS[i].x, FLOOR_TEX_COORDS[i].y);
        glVertex3f(FLOOR_VERTICES[i].x, FLOOR_VERTICES[i].y, FLOOR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Draw the mesa
    glBindTexture(GL_TEXTURE_2D, mesa_tex_id);
    glPushMatrix();
    glTranslatef(1, -0.5, 0);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < MESA_VERTEX_COUNT; i++)
    {
        glNormal3f(MESA_NORMALS[i].x*0.5, MESA_NORMALS[i].y, MESA_NORMALS[i].z);
        glTexCoord2f(MESA_TEX_COORDS[i].x *0.5, MESA_TEX_COORDS[i].y);
        glVertex3f(MESA_VERTICES[i].x*0.5, MESA_VERTICES[i].y, MESA_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Draw the chair in front of the door (chair1)
    glBindTexture(GL_TEXTURE_2D, chair_tex_id);
    glPushMatrix();
    glTranslatef(1, 0.5, -3);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < CHAIR_VERTEX_COUNT; i++)
    {
        glNormal3f(CHAIR_NORMALS[i].x * 10, CHAIR_NORMALS[i].y, CHAIR_NORMALS[i].z);
        glTexCoord2f(CHAIR_TEX_COORDS[i].x * 10, CHAIR_TEX_COORDS[i].y * 10);
        glVertex3f(CHAIR_VERTICES[i].x, CHAIR_VERTICES[i].y, CHAIR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);


    // Draw the middle chair (chair2)
    glBindTexture(GL_TEXTURE_2D, chair_tex_id);
    glPushMatrix();
    glRotatef(-90, 0.0f, 1.0f, 0.0f);
    glTranslatef(0, 0, -3.5);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < CHAIR_VERTEX_COUNT; i++)
    {
        glNormal3f(CHAIR_NORMALS[i].x * 10, CHAIR_NORMALS[i].y, CHAIR_NORMALS[i].z);
        glTexCoord2f(CHAIR_TEX_COORDS[i].x * 10, CHAIR_TEX_COORDS[i].y * 10);
        glVertex3f(CHAIR_VERTICES[i].x, CHAIR_VERTICES[i].y, CHAIR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw the chair next to the door (chair3)
    glBindTexture(GL_TEXTURE_2D, chair_tex_id);
    glPushMatrix();
    glRotatef(180, 0.0f, 1.0f, 0.0f);
    glTranslatef(-1, 0.5, -3);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < CHAIR_VERTEX_COUNT; i++)
    {
        glNormal3f(CHAIR_NORMALS[i].x * 10, CHAIR_NORMALS[i].y, CHAIR_NORMALS[i].z);
        glTexCoord2f(CHAIR_TEX_COORDS[i].x * 10, CHAIR_TEX_COORDS[i].y * 10);
        glVertex3f(CHAIR_VERTICES[i].x, CHAIR_VERTICES[i].y, CHAIR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw the freezer
    glBindTexture(GL_TEXTURE_2D, freezer_tex_id);
    glPushMatrix();
    glTranslatef(14, 1, 6.75);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < FREEZER_VERTEX_COUNT; i++)
    {
        glNormal3f(FREEZER_NORMALS[i].x, FREEZER_NORMALS[i].y, FREEZER_NORMALS[i].z);
        glTexCoord2f(FREEZER_TEX_COORDS[i].x, FREEZER_TEX_COORDS[i].y);
        glVertex3f(FREEZER_VERTICES[i].x, FREEZER_VERTICES[i].y, FREEZER_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw the mug 1 of chair1
    glPushMatrix();
    glTranslatef(-1.5, 0.8, 0.5);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < MUG_VERTEX_COUNT; i++)
    {
        glNormal3f(MUG_NORMALS[i].x, MUG_NORMALS[i].y, MUG_NORMALS[i].z);
        glTexCoord2f(MUG_TEX_COORDS[i].x, MUG_TEX_COORDS[i].y);
        glVertex3f(MUG_VERTICES[i].x, MUG_VERTICES[i].y, MUG_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the mug 2 of chair2
    glPushMatrix();
    glTranslatef(0.6, 0.8, 1.3);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < MUG_VERTEX_COUNT; i++)
    {
        glNormal3f(MUG_NORMALS[i].x, MUG_NORMALS[i].y, MUG_NORMALS[i].z);
        glTexCoord2f(MUG_TEX_COORDS[i].x, MUG_TEX_COORDS[i].y);
        glVertex3f(MUG_VERTICES[i].x, MUG_VERTICES[i].y, MUG_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the mug 3 of chair3
    glPushMatrix();
    glTranslatef(-1.5, 0.8, 3.45);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < MUG_VERTEX_COUNT; i++)
    {
        glNormal3f(MUG_NORMALS[i].x, MUG_NORMALS[i].y, MUG_NORMALS[i].z);
        glTexCoord2f(MUG_TEX_COORDS[i].x, MUG_TEX_COORDS[i].y);
        glVertex3f(MUG_VERTICES[i].x, MUG_VERTICES[i].y, MUG_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();
    
    // Draw the plate 1 of chair1
    glPushMatrix();
    glTranslatef(2.7, 0.8, 0);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < PLATE_VERTEX_COUNT; i++)
    {
        glNormal3f(PLATE_NORMALS[i].x, PLATE_NORMALS[i].y, PLATE_NORMALS[i].z);
        glTexCoord2f(PLATE_TEX_COORDS[i].x, PLATE_TEX_COORDS[i].y);
        glVertex3f(PLATE_VERTICES[i].x, PLATE_VERTICES[i].y, PLATE_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the plate 2 of chair2
    glPushMatrix();
    glTranslatef(4, 0.8, 1.5);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < PLATE_VERTEX_COUNT; i++)
    {
        glNormal3f(PLATE_NORMALS[i].x, PLATE_NORMALS[i].y, PLATE_NORMALS[i].z);
        glTexCoord2f(PLATE_TEX_COORDS[i].x, PLATE_TEX_COORDS[i].y);
        glVertex3f(PLATE_VERTICES[i].x, PLATE_VERTICES[i].y, PLATE_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the plate 3 of chair3
    glPushMatrix();
    glTranslatef(2.7, 0.8, 3);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < PLATE_VERTEX_COUNT; i++)
    {
        glNormal3f(PLATE_NORMALS[i].x, PLATE_NORMALS[i].y, PLATE_NORMALS[i].z);
        glTexCoord2f(PLATE_TEX_COORDS[i].x * 2, PLATE_TEX_COORDS[i].y);
        glVertex3f(PLATE_VERTICES[i].x, PLATE_VERTICES[i].y, PLATE_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Draw the fan
    glBindTexture(GL_TEXTURE_2D, fan_tex_id);
    glPushMatrix();
    glTranslatef(6, 8, 0);
    // glRotatef(90, 0.0f, 1.0f, 0.0f);
    glRotatef(fan_speed, 0.0f, 1.0f, 0.0f);
    // glTranslatef(-1.220647, -2.604736, -2.619632);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < FAN_VERTEX_COUNT; i++)
    {
        glNormal3f(FAN_NORMALS[i].x * 0.5, FAN_NORMALS[i].y * 0.5, FAN_NORMALS[i].z * 0.5);
        glTexCoord2f(FAN_TEX_COORDS[i].x * 0.5, FAN_TEX_COORDS[i].y * 0.5);
        glVertex3f(FAN_VERTICES[i].x * 0.5, FAN_VERTICES[i].y * 0.5, FAN_VERTICES[i].z * 0.5);
    }
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw the paiting
    glBindTexture(GL_TEXTURE_2D, painting_tex_id);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < PAINTING_VERTEX_COUNT; i++)
    {
        glNormal3f(PAINTING_NORMALS[i].x, PAINTING_NORMALS[i].y, PAINTING_NORMALS[i].z);
        glTexCoord2f(PAINTING_TEX_COORDS[i].x, PAINTING_TEX_COORDS[i].y);
        glVertex3f(PAINTING_VERTICES[i].x, PAINTING_VERTICES[i].y, PAINTING_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw the paiting
    glBindTexture(GL_TEXTURE_2D, walls_tex_id);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < WALLS_VERTEX_COUNT; i++)
    {
        glNormal3f(WALLS_NORMALS[i].x, WALLS_NORMALS[i].y, WALLS_NORMALS[i].z);
        glTexCoord2f(WALLS_TEX_COORDS[i].x, WALLS_TEX_COORDS[i].y);
        glVertex3f(WALLS_VERTICES[i].x, WALLS_VERTICES[i].y, WALLS_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();
}

void idle()
{
    // Forward movement
    int move_forward = KEYBOARD['w'] - KEYBOARD['s'];
    Vec3 fwd = forward(&CAM);

    fwd.x *= KEYBOARD['c'] ? move_forward * 2 : move_forward;
    fwd.y = 0.0f; // Projects fwd in the xz plane
    fwd.z *= KEYBOARD['c'] ? move_forward * 2 : move_forward;

    main_door();
    side_door();
    windows();
    move_fan();

    // Lateral movement
    int move_right = KEYBOARD['d'] - KEYBOARD['a'];
    Vec3 rgt = right(&CAM);

    rgt.x *= move_right;
    rgt.z *= move_right;

    CAM.position.x += 0.1f * (fwd.x + rgt.x);
    CAM.position.z += 0.1f * (fwd.z + rgt.z);

    glutPostRedisplay();
}

void motion(int x, int y)
{
    static int wrap = 0;
    Vec2 delta;

    if (!wrap)
    {
        delta.x = x - WINDOW_CENTER.x;
        delta.y = y - WINDOW_CENTER.y;

        // If delta.y > 0 -> camera moving down
        // If delta.y < 0 -> camera moving up
        CAM.rotation.x += delta.y > 0 ? 1.0f : (delta.y < 0 ? -1.0f : 0.0f);
        // If delta.x > 0 -> camera moving left
        // If delta.x < 0 -> camera moving right
        CAM.rotation.y -= delta.x > 0 ? 1.0f : (delta.x < 0 ? -1.0f : 0.0f);

        wrap = 1;
        // Maintaing the mouse pointer in the middle
        glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);
    }
    else
    {
        wrap = 0;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    // Escape key
    if(key == 27)
        glutLeaveMainLoop();

    // Setting the key as pressed
    KEYBOARD[tolower(key)] = 1;

    if(tolower(key) == '1' || tolower(key) == 'e')
        open_main_door *= -1;
    if(tolower(key) == '2')
        open_side_door *= -1;
    if(tolower(key) == '3')
        open_windows *= -1;
}

void keyboard_up(unsigned char key, int x, int y)
{
    // Resetting the key as not pressed
    KEYBOARD[tolower(key)] = 0;
}

void reshape(int width, int height)
{
    float aspect = (float) width / (float) height;
    WINDOW_SIZE.x = width;
    WINDOW_SIZE.y = height;
    WINDOW_CENTER.x = width / 2;
    WINDOW_CENTER.y = height / 2;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, aspect, ZNEAR, ZFAR);
}


/*---------------------------------------------------------*/
/*------------------- DRAWING FUNCTIONS -------------------*/
/*---------------------------------------------------------*/
void draw_grid(int n)
{
    int i;

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for(i = -n;i < n;i++){
        float d = (float) i;
        // Parallel to x-axis
        glVertex3f(-n, 0.0f, d);
        glVertex3f(n, 0.0f, d);
        // Parallel to z-axis
        glVertex3f(d, 0.0f, -n);
        glVertex3f(d, 0.0f, n);
    }
    glEnd();
}

void main_door()
{
    if(open_main_door == 1)
    {
        if(main_door_angle > -110)
            main_door_angle -= 3;
        else
            main_door_angle = -110;
    }
    else if(open_main_door == -1)
    {
        if(main_door_angle < 0)
            main_door_angle += 3;
        else
            main_door_angle = 0;
    }
}

void side_door()
{
    if(open_side_door == 1)
    {
        if(side_door_angle > -95)
            side_door_angle -= 3;
        else
            side_door_angle = -95;
    }
    else if(open_side_door == -1)
    {
        if(side_door_angle < 0)
            side_door_angle += 3;
        else
            side_door_angle = 0;
    }
}

void windows()
{
    if(open_windows == 1)
    {
        if(window_angle > -145)
            window_angle -= 3;
        else
            window_angle = -145;
    }
    else
    {
        if(window_angle < 0)
            window_angle += 3;
        else
            window_angle = 0;
    }
}

void move_fan()
{
    if(fan_speed > 360)
        fan_speed = 0;
    
    fan_speed += 1;
}


/*--------------------------------------------------------*/
/*-------------------- MATH FUNCTIONS --------------------*/
/*--------------------------------------------------------*/
Vec3 forward(Transform* t) {
    Vec3 v;
    float a = t->rotation.x * DEG2RAD * 2;
    float b = t->rotation.y * DEG2RAD * 2;
    float c = t->rotation.z * DEG2RAD * 2;

    v.x = -(sin(c) * sin(a) + cos(c) * sin(b) * cos(a));
    v.y = (-cos(c) * sin(a) + sin(c) * sin(b) * cos(a));
    v.z = -(cos(b) * cos(a));

    return v;
}

Vec3 up(Transform* t) {
    Vec3 v;
    float a = t->rotation.x * DEG2RAD * 2;
    float b = t->rotation.y * DEG2RAD * 2;
    float c = t->rotation.z * DEG2RAD * 2;

    v.x = -sin(c) * cos(a) + cos(c) * sin(b) * sin(a);
    v.y = cos(c) * cos(a) + sin(c) * sin(b) * sin(a);
    v.z = cos(b) * sin(a);

    return v;
}

Vec3 right(Transform* t) {
    Vec3 v;
    float b = t->rotation.y * DEG2RAD * 2;
    float c = t->rotation.z * DEG2RAD * 2;

    v.x = cos(c) * cos(b);
    v.y = sin(c) * cos(b);
    v.z = -sin(b);

    return v;
}
