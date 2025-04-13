#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Variabel global
int windowWidth = 1200;                // Lebar jendela aplikasi
int windowHeight = 900;               // Tinggi jendela aplikasi
float rotationX = 0.0f;               // Rotasi pada sumbu X
float rotationY = 0.0f;               // Rotasi pada sumbu Y
int projectionMode = 0;                // Mode proyeksi saat ini
float angle = 0.0;                       // Sudut rotasi objek
								      // 4=dimetric, 5=trimetric, 6=1pt, 7=2pt, 8=3pt
const int TOTAL_PROJECTIONS = 9;      // Jumlah total mode proyeksi
const char* projectionNames[] = {"Plan View", "Front View", "Side View", "Isometric", 
                                "Dimetric", "Trimetric", "1-Point Perspective", 
                                "2-Point Perspective", "3-Point Perspective"};

// Warna-warna dasar
float red[] = {1.0f, 0.0f, 0.0f};         // Merah
float green[] = {0.0f, 1.0f, 0.0f};       // Hijau
float blue[] = {0.0f, 0.0f, 1.0f};        // Biru
float yellow[] = {1.0f, 1.0f, 0.0f};      // Kuning
float cyan[] = {0.0f, 1.0f, 1.0f};        // Cyan
float magenta[] = {1.0f, 0.0f, 1.0f};     // Magenta
float white[] = {1.0f, 1.0f, 1.0f};       // Putih
float gray[] = {0.5f, 0.5f, 0.5f};        // Abu-abu
float darkBrown[] = {0.4f, 0.2f, 0.0f};   // Coklat tua

// warna kursi
float brightOrange[] = {1.0f, 0.6f, 0.0f};       // Warna utama kursi (oranye cerah)
float lighterOrange[] = {1.0f, 0.7f, 0.2f};      // Warna alas kursi (oranye muda)
float darkerOrange[] = {0.8f, 0.4f, 0.0f};       // Warna kaki kursi (oranye tua)
float orangeHighlight[] = {1.0f, 0.8f, 0.4f};    // Warna aksen (oranye terang)
float orangeShadow[] = {0.7f, 0.35f, 0.0f};      // Warna bayangan (oranye gelap)
float black[] = {0.1f, 0.1f, 0.1f};              // Warna garis tepi (hitam)

const GLfloat light_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};    // Reduced ambient light
const GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};    // Reduced diffuse light
const GLfloat light_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};   // Reduced specular light
const GLfloat light_position[] = {0.0f, 20.0f, 10.0f, 0.0f}; // Directional light
const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

void lighting()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    // Enable shadow calculation
    glEnable(GL_LIGHT_MODEL_LOCAL_VIEWER);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void Grid() {
    // Fungsi untuk membuat grid di "lantai"
    double i;
    const float Z_MIN = -500, Z_MAX = 500;
    const float X_MIN = -500, X_MAX = 500;
    const float gap = 1.5;

    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for(i=Z_MIN;i<Z_MAX;i+=gap)
    {
    glVertex3f(i, 0, Z_MIN);
    glVertex3f(i, 0, Z_MAX);
    }
    for(i=X_MIN;i<X_MAX;i+=gap)
    {
    glVertex3f(X_MIN, 0, i);
    glVertex3f(X_MAX, 0, i);
    }
    glEnd();
    }

// Fungsi untuk menggambar kubus dengan garis tepi
void drawCube(float size, float lineWidth) {
    // Menyimpan warna saat ini
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    
    glBegin(GL_QUADS);
    // Sisi depan
    glVertex3f(-size/2, -size/2, size/2);
    glVertex3f(size/2, -size/2, size/2);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(-size/2, size/2, size/2);
    
    // Sisi belakang
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(size/2, size/2, -size/2);
    glVertex3f(size/2, -size/2, -size/2);
    
    // Sisi kiri
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(-size/2, -size/2, size/2);
    glVertex3f(-size/2, size/2, size/2);
    glVertex3f(-size/2, size/2, -size/2);
    
    // Sisi kanan
    glVertex3f(size/2, -size/2, -size/2);
    glVertex3f(size/2, size/2, -size/2);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(size/2, -size/2, size/2);
    
    // Sisi atas
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(-size/2, size/2, size/2);
    glVertex3f(size/2, size/2, size/2);
    glVertex3f(size/2, size/2, -size/2);
    
    // Sisi bawah
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(size/2, -size/2, -size/2);
    glVertex3f(size/2, -size/2, size/2);
    glVertex3f(-size/2, -size/2, size/2);
    glEnd();
    
    // Garis tepi
    if (lineWidth > 0) {
        glColor3fv(black);            // hitam untuk garis tepi
        glLineWidth(lineWidth);       // ketebalan garis
        
        // outline depan
        glBegin(GL_LINE_LOOP);
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, size/2);
        glEnd();
        
        // outline belakang
        glBegin(GL_LINE_LOOP);
        glVertex3f(-size/2, -size/2, -size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glVertex3f(size/2, size/2, -size/2);
        glVertex3f(size/2, -size/2, -size/2);
        glEnd();
        
        // penghubung
        glBegin(GL_LINES);
        glVertex3f(-size/2, -size/2, size/2);
        glVertex3f(-size/2, -size/2, -size/2);
        
        glVertex3f(size/2, -size/2, size/2);
        glVertex3f(size/2, -size/2, -size/2);
        
        glVertex3f(size/2, size/2, size/2);
        glVertex3f(size/2, size/2, -size/2);
        
        glVertex3f(-size/2, size/2, size/2);
        glVertex3f(-size/2, size/2, -size/2);
        glEnd();
        
        // warna asli
        glColor4fv(currentColor);
        glLineWidth(1.0f);    // ketebalan garis default
    }
}

// Menggambar balok persegi panjang
void drawRectPrism(float width, float height, float depth, float lineWidth) {
    glPushMatrix();
    glScalef(width, height, depth); 
    drawCube(1.0f, lineWidth);
    glPopMatrix();
}

void drawSolidCylinder(GLfloat radius, GLfloat height, int slices, int stacks)
{
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluCylinder(quadric, radius, radius, height, slices, stacks);
    gluDeleteQuadric(quadric);
}
void drawCar(float x, float z)
{
    glPushMatrix();
    glTranslatef(x, -1, z);
    glScalef(4, 4, 4); // Scale the whole car by 1.5 in all dimensions

    // Car body color
    glColor3f(0.8f, 0.2f, 0.0f); // Example: Orange color

    // Front Body
    glBegin(GL_QUADS);
    // Top
    glVertex3f(0.2f, 0.4f, 0.6f);
    glVertex3f(0.6f, 0.5f, 0.6f);
    glVertex3f(0.6f, 0.5f, 0.2f);
    glVertex3f(0.2f, 0.4f, 0.2f);

    // Bottom
    glVertex3f(0.2f, 0.2f, 0.6f);
    glVertex3f(0.6f, 0.2f, 0.6f);
    glVertex3f(0.6f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);

    // Front
    glVertex3f(0.2f, 0.2f, 0.6f);
    glVertex3f(0.2f, 0.4f, 0.6f);
    glVertex3f(0.2f, 0.4f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);

    // Back
    glVertex3f(0.6f, 0.2f, 0.6f);
    glVertex3f(0.6f, 0.5f, 0.6f);
    glVertex3f(0.6f, 0.5f, 0.2f);
    glVertex3f(0.6f, 0.2f, 0.2f);

    // Left
    glVertex3f(0.2f, 0.2f, 0.6f);
    glVertex3f(0.6f, 0.2f, 0.6f);
    glVertex3f(0.6f, 0.5f, 0.6f);
    glVertex3f(0.2f, 0.4f, 0.6f);

    // Right
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(0.6f, 0.2f, 0.2f);
    glVertex3f(0.6f, 0.5f, 0.2f);
    glVertex3f(0.2f, 0.4f, 0.2f);
    glEnd();

    // Middle Body Top Cover
    glBegin(GL_QUADS);
    glVertex3f(0.7f, 0.65f, 0.6f);
    glVertex3f(0.7f, 0.65f, 0.2f);
    glVertex3f(1.7f, 0.65f, 0.2f);
    glVertex3f(1.7f, 0.65f, 0.6f);
    glEnd();

    // Back Guard
    glColor3f(0.8f, 0.2f, 0.0f); // Match car body color
    glBegin(GL_QUADS);
    // Front
    glVertex3f(1.8f, 0.5f, 0.6f);
    glVertex3f(1.8f, 0.5f, 0.2f);
    glVertex3f(2.1f, 0.4f, 0.2f);
    glVertex3f(2.1f, 0.4f, 0.6f);

    // Bottom
    glVertex3f(2.1f, 0.2f, 0.6f);
    glVertex3f(2.1f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.2f, 0.6f);

    // Back
    glVertex3f(2.1f, 0.4f, 0.6f);
    glVertex3f(2.1f, 0.4f, 0.2f);
    glVertex3f(2.1f, 0.2f, 0.2f);
    glVertex3f(2.1f, 0.2f, 0.6f);

    // Left
    glVertex3f(1.8f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.5f, 0.2f);
    glVertex3f(2.1f, 0.4f, 0.2f);
    glVertex3f(2.1f, 0.2f, 0.2f);

    // Right
    glVertex3f(1.8f, 0.2f, 0.6f);
    glVertex3f(1.8f, 0.5f, 0.6f);
    glVertex3f(2.1f, 0.4f, 0.6f);
    glVertex3f(2.1f, 0.2f, 0.6f);
    glEnd();

    // Middle Body
    glBegin(GL_QUADS);
    // Front
    glVertex3f(0.6f, 0.5f, 0.6f);
    glVertex3f(0.6f, 0.2f, 0.6f);
    glVertex3f(1.8f, 0.2f, 0.6f);
    glVertex3f(1.8f, 0.5f, 0.6f);

    // Bottom
    glVertex3f(0.6f, 0.2f, 0.6f);
    glVertex3f(0.6f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.2f, 0.6f);

    // Back
    glVertex3f(0.6f, 0.5f, 0.2f);
    glVertex3f(0.6f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.2f, 0.2f);
    glVertex3f(1.8f, 0.5f, 0.2f);
    glEnd();

    // Windows
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    // Front Windows
    glVertex3f(0.77f, 0.63f, 0.2f);
    glVertex3f(0.75f, 0.5f, 0.2f);
    glVertex3f(1.2f, 0.5f, 0.2f);
    glVertex3f(1.22f, 0.63f, 0.2f);

    glVertex3f(1.27f, 0.63f, 0.2f);
    glVertex3f(1.25f, 0.5f, 0.2f);
    glVertex3f(1.65f, 0.5f, 0.2f);
    glVertex3f(1.67f, 0.63f, 0.2f);

    // Back Windows
    glVertex3f(0.77f, 0.63f, 0.6f);
    glVertex3f(0.75f, 0.5f, 0.6f);
    glVertex3f(1.2f, 0.5f, 0.6f);
    glVertex3f(1.22f, 0.63f, 0.6f);

    glVertex3f(1.27f, 0.63f, 0.6f);
    glVertex3f(1.25f, 0.5f, 0.6f);
    glVertex3f(1.65f, 0.5f, 0.6f);
    glVertex3f(1.67f, 0.63f, 0.6f);
    glEnd();

    // Window Separations
    glColor3f(0.8f, 0.2f, 0.0f); // Match car body color
    glBegin(GL_QUADS);
    // Vertical Separations
    glVertex3f(0.7f, 0.65f, 0.2f);
    glVertex3f(0.7f, 0.5f, 0.2f);
    glVertex3f(0.75f, 0.5f, 0.2f);
    glVertex3f(0.77f, 0.65f, 0.2f);

    glVertex3f(1.2f, 0.65f, 0.2f);
    glVertex3f(1.2f, 0.5f, 0.2f);
    glVertex3f(1.25f, 0.5f, 0.2f);
    glVertex3f(1.27f, 0.65f, 0.2f);

    glVertex3f(1.65f, 0.65f, 0.2f);
    glVertex3f(1.65f, 0.5f, 0.2f);
    glVertex3f(1.7f, 0.5f, 0.2f);
    glVertex3f(1.7f, 0.65f, 0.2f);

    // Horizontal Separations
    glVertex3f(0.75f, 0.65f, 0.2f);
    glVertex3f(0.75f, 0.63f, 0.2f);
    glVertex3f(1.7f, 0.63f, 0.2f);
    glVertex3f(1.7f, 0.65f, 0.2f);

    glVertex3f(0.75f, 0.65f, 0.6f);
    glVertex3f(0.75f, 0.63f, 0.6f);
    glVertex3f(1.7f, 0.63f, 0.6f);
    glVertex3f(1.7f, 0.65f, 0.6f);

    // Vertical Separations (Back Windows)
    glVertex3f(0.7f, 0.65f, 0.6f);
    glVertex3f(0.7f, 0.5f, 0.6f);
    glVertex3f(0.75f, 0.5f, 0.6f);
    glVertex3f(0.77f, 0.65f, 0.6f);

    glVertex3f(1.2f, 0.65f, 0.6f);
    glVertex3f(1.2f, 0.5f, 0.6f);
    glVertex3f(1.25f, 0.5f, 0.6f);
    glVertex3f(1.27f, 0.65f, 0.6f);

    glVertex3f(1.65f, 0.65f, 0.6f);
    glVertex3f(1.65f, 0.5f, 0.6f);
    glVertex3f(1.7f, 0.5f, 0.6f);
    glVertex3f(1.7f, 0.65f, 0.6f);
    glEnd();

    // Top of Middle Body Sides
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(0.6f, 0.5f, 0.6f);
    glVertex3f(0.6f, 0.5f, 0.2f);
    glVertex3f(0.7f, 0.65f, 0.2f);
    glVertex3f(0.7f, 0.65f, 0.6f);

    glVertex3f(1.7f, 0.65f, 0.6f);
    glVertex3f(1.7f, 0.65f, 0.2f);
    glVertex3f(1.8f, 0.5f, 0.2f);
    glVertex3f(1.8f, 0.5f, 0.6f);
    glEnd();

    // Triangles on Top of Windows
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.6f, 0.5f, 0.6f);
    glVertex3f(0.7f, 0.65f, 0.6f);
    glVertex3f(0.7f, 0.5f, 0.6f);

    glVertex3f(0.6f, 0.5f, 0.2f);
    glVertex3f(0.7f, 0.65f, 0.2f);
    glVertex3f(0.7f, 0.5f, 0.2f);

    glVertex3f(1.7f, 0.65f, 0.2f);
    glVertex3f(1.8f, 0.5f, 0.2f);
    glVertex3f(1.7f, 0.5f, 0.2f);

    glVertex3f(1.7f, 0.65f, 0.6f);
    glVertex3f(1.8f, 0.5f, 0.6f);
    glVertex3f(1.7f, 0.5f, 0.6f);
    glEnd();

    // Ignition System
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.7f);
    glTranslatef(1.65f, 0.2f, 0.3f);
    glRotatef(90.0f, 0, 1, 0);
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, 0.02, 0.03, 0.5, 10, 10);
    gluDeleteQuadric(quad);
    glPopMatrix();

    // Wheels
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray

    // Function to draw a wheel
    auto drawWheel = [&](float x, float z)
    {
        glPushMatrix();
        glColor3f(0.2f, 0.2f, 0.2f);
        glTranslatef(x, 0.2f, z);
        glRotatef(angle, 0, 0, 1); // Rotate the wheel
        glutSolidTorus(0.025, 0.07, 10, 25);
        glPopMatrix();

        // Draw the rim as a full circle (torus)
        glColor3f(0.9f, 0.9f, 0.9f); // Light gray for the rim
        glPushMatrix();
        glTranslatef(x, 0.2f, z);
        glRotatef(angle, 0, 0, 1);          // Rotate the wheel
        glutSolidTorus(0.02, 0.05, 10, 25); // Inner radius, outer radius, sides, rings
        glPopMatrix();

        // use "for" loop to draw the spokes
        for (int i = 0; i < 360; i += 45)
        {
            glPushMatrix();
            glColor3f(0.9f, 0.9f, 0.9f); // Light gray for the spokes
            glTranslatef(x, 0.2f, z);
            glRotatef(i, 0, 0, 1);     // Rotate the wheel
            glRotatef(90.0f, 0, 1, 0); // Rotate to align with the wheel
            gluCylinder(gluNewQuadric(), 0.005, 0.005, 0.06, 10, 10);
            glPopMatrix();
        }
    };

    // Draw each wheel
    drawWheel(0.6f, 0.62f);
    drawWheel(0.6f, 0.18f);
    drawWheel(1.7f, 0.18f);
    drawWheel(1.7f, 0.62f);

    glPopMatrix();
}


// Sumbu koordinat
void drawAxes() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    
    // Sumbu X (merah)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    
    // Sumbu Y (hijau)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    
    // Sumbu Z (biru)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();
    glLineWidth(1.0f);
}


void setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspectRatio = (float)windowWidth / (float)windowHeight;
    
    switch (projectionMode) {
        case 0: // Tampilan plan (ortografik dari atas)
            glOrtho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0f, 10.0f, 0.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f);
            break;
            
        case 1: // Tampilan depan (ortografik)
            glOrtho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0f, 0.0f, 10.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
            
        case 2: // Tampilan samping (ortografik)
            glOrtho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(10.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
            
        case 3: // Isometrik (semua sudut sama)
            glOrtho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(5.77f, 5.77f, 5.77f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
            
        case 4: // Dimetrik (dua sudut sama)
            glOrtho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(8.66f, 5.0f, 5.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
            
        case 5: // Trimetrik (semua sudut berbeda)
            glOrtho(-10.0f * aspectRatio, 10.0f * aspectRatio, -10.0f, 10.0f, -100.0f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(7.0f, 4.0f, 6.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
            
        case 6: // Perspektif 1-Titik
            gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0.0f, 0.0f, 10.0f, 
                      0.0f, 0.0f, 0.0f,  
                      0.0f, 1.0f, 0.0f); 
            break;
            
        case 7: // Perspektif 2-Titik
            gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(8.0f, 0.0f, 8.0f, 
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
            
        case 8: // Perspektif 3-Titik
            gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(8.0f, 5.0f, 8.0f,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            break;
    }
}

bool showOverlay = false;

void drawOverlay() {
    // Tampilkan nama perspektif saat ini di kanan atas layar
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // Warna teks
    const char* currentProjection = projectionNames[projectionMode];
    int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)currentProjection);
    glRasterPos2i(windowWidth - textWidth - 20, windowHeight - 20);
    for (const char* c = currentProjection; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // Tampilkan overlay jika diaktifkan
    if (showOverlay) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, windowWidth, 0, windowHeight);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f); // Warna teks
        const char* controls[] = {
            "Controls:",
            "Space: Change projection mode",
            "1-9: Select specific projection",
            "Up/Down: Rotate around X-axis",
            "Left/Right: Rotate around Y-axis",
            "R: Reset rotation",
            "ESC: Exit"
        };
        int yOffset = 30;

        for (int i = 0; i < 7; i++) {
            glRasterPos2i(20, windowHeight - 40 - yOffset * i);
            for (const char* c = controls[i]; *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            }
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
}


void display() {
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    setProjection();

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); 
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); 

    drawAxes();
    
    Grid();
    drawCar(-5,-1.5);
    drawOverlay();
    glutSwapBuffers();
}

// perubahan ukuran jendela
void reshape(int w, int h) {
    windowWidth = w; 
    windowHeight = h;
    glViewport(0, 0, w, h);
}

// input keyboard
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Tombol ESC - keluar dari program
            exit(0);
            break;
        case ' ': // Tombol spasi - ganti mode proyeksi
            projectionMode = (projectionMode + 1) % TOTAL_PROJECTIONS;
            break;
        case 'r': // Reset rotasi
            rotationX = 0.0f;
            rotationY = 0.0f;
            break;

        case '\t': // Toggle overlay
            showOverlay = !showOverlay;
            break;

        case '1': case '2': case '3': case '4': case '5': 
            case '6': case '7': case '8': case '9':
                projectionMode = key - '1';
                break;
    }
    glutPostRedisplay();
}

// Fungsi untuk menangani tombol khusus
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    // panah atas - rotasi ke atas pada sumbu X
            rotationX += 5.0f;
            break;
        case GLUT_KEY_DOWN:  // panah bawah - rotasi ke bawah pada sumbu X
            rotationX -= 5.0f;
            break;
        case GLUT_KEY_LEFT:  // panah kiri - rotasi ke kiri pada sumbu Y
            rotationY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT: // panah kanan - rotasi ke kanan pada sumbu Y
            rotationY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

// Inisialisasi OpenGL
void init() {
    glEnable (GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tugas GKV - Proyeksi 3D");
    glutSetCursor(GLUT_CURSOR_NONE); // Menghilangkan cursor
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();
    return 0;
}

