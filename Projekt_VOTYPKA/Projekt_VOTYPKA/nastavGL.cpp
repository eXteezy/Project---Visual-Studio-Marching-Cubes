#include "nastavGL.h"

// funkce pro vykreslení
void nastavGL::zmenaCele(int angle1, int angle2, int angle3, int translateX, int translateY, float zoom)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// rotace
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(angle1, 1.0, 0.0, 0.0);
	glRotatef(angle2, 0.0, 1.0, 0.0);
	glRotatef(angle3, 0.0, 0.0, 1.0);
	// posun
	glTranslatef(translateY, translateX, 0);
	// zoom
	glScalef(zoom, zoom, zoom);
	glColor3f(1.0, 1.0, 1.0); // bílá barva
}
// funkce pro zmìnu okna
void nastavGL::zmenaOkna(int sirka, int vyska)
{
	// zvìtšení/zmenšení na danou šíøku a výšku
	glViewport(0, 0, sirka, vyska);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glOrtho(0, sirka, 0, vyska, -200, 200);
	glScalef(1, -1, 1);
	glTranslatef(0, -vyska, 0);
}
// funkce pro obnovení GL na defaultní hodnoty
void nastavGL::obnov()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 600, 0, 600, 0, 600);
	glScalef(1, -1, 1);
	glTranslatef(256, -1.5 * 256, -256);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
