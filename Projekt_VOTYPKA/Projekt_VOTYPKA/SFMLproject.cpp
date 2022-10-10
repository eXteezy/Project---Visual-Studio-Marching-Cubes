// SFMLproject.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

/* ZADÁNÍ
   Vytvořte program, který implementuje metodu "marching cubes" (pochodující kostky) pro zobrazení objemových dat.
   Program by měl umožňovat:
	1. Načítat sady řezů uložené v běžných obrazových formátech (png, jpg, gif apod. ).
	2. Na základě nastaveného prahu generovat souřadnice trojúhelníků aproximující povrch.
	3. Stanovení normálových vektorů pro jednotlivé polygony.
	4. Vykreslení polygonových dat do okna programu.
	5. Základní manipulaci s objektem (rotace, posun, volitelně zoom).
	6. Osvětlení scény
	7. Základní ovládání okna aplikace (změna velikosti okna, přepínání celoobrázkového režimu 
*/

// KNIHOVNY potřebné pro běh programu
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <stdlib.h>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include "stdio.h"
#include "math.h"
#include "nastavGL.h"
#include "tabulka.h"

using namespace std;
using namespace sf;

// GLOBÁLNÍ PROMĚNNÉ
// práh a zoom
int threshold = 115;
float zoom = 1.0;
// rotace
int angle1 = 0;
int angle2 = 0;
int angle3 = 0;
// posun
int translateX = 0;
int translateY = 0;
// bool
bool isFull = false; // fullscreen
// bool isThresh = false; // práh
bool isLight = true; // zapnutí a vypnutí osvětlení scény
// definování xvert, yvert, zvert
vector<float> xvert;
vector<float> yvert;
vector<float> zvert;
// definování normál v ose x, y a z
float xnorm, ynorm, znorm;

int main()
{
	// nastavení třídy nastavGL jako nastaveni
	nastavGL nastaveni;
	// NAČTENÍ OBRÁZKŮ - celkem 99
	Image obrazky[99];
	for (int i = 0; i < 99; i++)
	{
		// obrázek jednomístné číslo
		if (i < 9)
		{
			// ZDE ZMĚNIT CESTU PRO OBRÁZKY --->
			if (!obrazky[i].loadFromFile("C:/ŠKOLA/BIOMEDICINA/9.SEMESTR/MPC-VIZ/PROJEKT/ProjektData/CT00" + to_string(i + 1) + ".png"))
			{
				cout << "Error. Obrazek se nepodarilo nacist." << endl;
				return -1;
			}
		}
		// obrázek dvojmístné číslo
		else
		{
			// ZDE ZMĚNIT CESTU PRO OBRÁZKY --->
			if (!obrazky[i].loadFromFile("C:/ŠKOLA/BIOMEDICINA/9.SEMESTR/MPC-VIZ/PROJEKT/ProjektData/CT0" + to_string(i + 1) + ".png"))
			{
				cout << "Error. Obrazek se nepodarilo nacist." << endl;
				return -1;
			}
		}
	}
	// VYPSÁNÍ DO KONZOLE
	cout << "Obrazky byly nacteny" << endl;
	// zjištění velikosti obrázku
	Vector2u rozmer = obrazky[0].getSize();
	cout << "Obrazky maji rozmery v x: " << rozmer.x << " a y: " << rozmer.y << "; nastaveny prah je: " << threshold << endl;
	cout << "Ovladani programu: " <<  endl;
	cout << " - Klavesou ESC se ukonci aplikace." << endl;
	cout << " - Klavesou W se prepina do fullscreen rezimu a zpet." << endl;
	cout << " - Klavesou A se provede priblizeni a klavesou S oddaleni." << endl;
	cout << " - Klavesou F se provede rotace ve vsech uhlech a klavesou G v opacnem smeru." << endl;
	cout << " - Klavesou H se provede rotace ve smeru x a klavesou J v opacnem smeru." << endl;
	cout << " - Klavesou K se provede rotace ve smeru y a klavesou J v opacnem smeru." << endl;
	cout << " - Klavesou Y se provede rotace ve smeru z a klavesou X v opacnem smeru." << endl;
	cout << " - Klavesy C, V, B a N slouzi pro posun." << endl;
	cout << " - Klavesou M se zapne / vypne svetlo." << endl;
	cout << " - Klavesa D prepne na defaultni hodnoty - vsechny rotace, translace a priblizeni se vynuluji." << endl;

	// MARCHING CUBES algoritmus
	int sirka = rozmer.x;
	int vyska = rozmer.y;
	// 8 bodů pro krychli
	int pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8;
	
	xvert.clear();
	yvert.clear();
	zvert.clear();

	// definování dvou řezů následujících po sobě jako dva obrázky
	Image obrazek1;
	Image obrazek2;
	for (int pic = 0; pic < 98; pic++)
	{
		// načtení dvou řezů následujících po sobě
		obrazek1 = obrazky[pic];
		obrazek2 = obrazky[pic + 1];
		// vyhodnocování vrcholů 
		for (int j = 0; j < sirka - 1; j++)
		{
			for (int k = 0; k < vyska - 1; k++)
			{
				// zisk šedotónu -> každá složka násobena příšlusnou hodnotou -> POZN: možno definovat r, g, b jako globální proměnné
				pt1 = (int)(float)obrazek2.getPixel(j, k).r * 0.299 + (float)obrazek2.getPixel(j, k).g * 0.587 + (float)obrazek2.getPixel(j, k).b * 0.114;
				pt2 = (int)(float)obrazek2.getPixel(j, k + 1).r * 0.299 + (float)obrazek2.getPixel(j, k + 1).g * 0.587 + (float)obrazek2.getPixel(j, k + 1).b * 0.114;
				pt3 = (int)(float)obrazek2.getPixel(j + 1, k + 1).r * 0.299 + (float)obrazek2.getPixel(j + 1, k + 1).g * 0.587 + (float)obrazek2.getPixel(j + 1, k + 1).b * 0.114;
				pt4 = (int)(float)obrazek2.getPixel(j + 1, k).r * 0.299 + (float)obrazek2.getPixel(j + 1, k).g * 0.587 + (float)obrazek2.getPixel(j + 1, k).b * 0.114;
				pt5 = (int)(float)obrazek1.getPixel(j, k).r * 0.299 + (float)obrazek1.getPixel(j, k).g * 0.587 + (float)obrazek1.getPixel(j, k).b * 0.114;
				pt6 = (int)(float)obrazek1.getPixel(j, k + 1).r * 0.299 + (float)obrazek1.getPixel(j, k + 1).g * 0.587 + (float)obrazek1.getPixel(j, k + 1).b * 0.114;
				pt7 = (int)(float)obrazek1.getPixel(j + 1, k + 1).r * 0.299 + (float)obrazek1.getPixel(j + 1, k + 1).g * 0.587 + (float)obrazek1.getPixel(j + 1, k + 1).b * 0.114;
				pt8 = (int)(float)obrazek1.getPixel(j + 1, k).r * 0.299 + (float)obrazek1.getPixel(j + 1, k).g * 0.587 + (float)obrazek1.getPixel(j + 1, k).b * 0.114;
				// určení zda daný bod náleží do objektu nebo ne
				int cube = 0;
				if (pt1 < threshold) cube |= 1;
				if (pt2 < threshold) cube |= 2;
				if (pt3 < threshold) cube |= 4;
				if (pt4 < threshold) cube |= 8;
				if (pt5 < threshold) cube |= 16;
				if (pt6 < threshold) cube |= 32;
				if (pt7 < threshold) cube |= 64;
				if (pt8 < threshold) cube |= 128;
				// interpolace získá interpolaci mezi vrcholy
				float interpolace; 
				for (int l = 0; l < 16; l++)
				{
					if (tabulka[cube][l] == -1)
						break;
					else
					{
						int edgeTable = tabulka[cube][l];
						switch (edgeTable)
						{
						case 0:
							// pro zapsání vrcholu jsou vždy v závorce dvě proměnné, které se navzájem vyruší, proto lze později zapsat pouze 1
							interpolace = j + (threshold - pt1) * (j + 1 - j) / (pt2 - pt1); 
							// zapsání do proměnné xvert, yvert a zvert
							xvert.push_back(interpolace);
							yvert.push_back(k);
							zvert.push_back(pic);
							break;
						case 1:
							interpolace = k + (threshold - pt2) / (pt3 - pt2);
							xvert.push_back(j);
							yvert.push_back(interpolace);
							zvert.push_back(pic);
							break;
						case 2:
							interpolace = j + 1 + (threshold - pt3) / (pt4 - pt3);
							xvert.push_back(interpolace);
							yvert.push_back(k);
							zvert.push_back(pic);
							break;
						case 3:
							interpolace = k + 1 + (threshold - pt4) / (pt1 - pt4);
							xvert.push_back(j);
							yvert.push_back(interpolace);
							zvert.push_back(pic);
							break;
						case 4:
							interpolace = j + (threshold - pt5) / (pt6 - pt5);
							xvert.push_back(interpolace);
							yvert.push_back(k);
							zvert.push_back(pic);
							break;
						case 5:
							interpolace = k + (threshold - pt6) / (pt7 - pt6);
							xvert.push_back(j);
							yvert.push_back(interpolace);
							zvert.push_back(pic);
							break;
						case 6:
							interpolace = j + 1 + (threshold - pt7) / (pt8 - pt7);
							xvert.push_back(interpolace);
							yvert.push_back(k);
							zvert.push_back(pic);
							break;
						case 7:
							interpolace = k + 1 + (threshold - pt8) / (pt5 - pt8);
							xvert.push_back(j);
							yvert.push_back(interpolace);
							zvert.push_back(pic);
							break;
						case 8:
							interpolace = pic + (threshold - pt5) / (pt1 - pt5);
							xvert.push_back(j);
							yvert.push_back(k);
							zvert.push_back(interpolace);
							break;
						case 9:
							interpolace = pic + (threshold - pt6) / (pt2 - pt6);
							xvert.push_back(j);
							yvert.push_back(k);
							zvert.push_back(interpolace);
							break;
						case 10:
							interpolace = pic + (threshold - pt7) / (pt3 - pt7);
							xvert.push_back(j);
							yvert.push_back(k);
							zvert.push_back(interpolace);
							break;
						case 11:
							interpolace = pic + (threshold - pt8) / (pt4 - pt8);
							xvert.push_back(j);
							yvert.push_back(k);
							zvert.push_back(interpolace);
							break;
						}
					}
				}
			}
		}
	}
	// definování proměnných pro DEFAULTNÍ nastavení posunů a rotací (viz. cyklus WHILE dále)
	int angle1Default;
	int angle2Default;
	int angle3Default;
	int translateXDefault;
	int translateYDefault;

	// nastavení SVĚTLA - pozice
	GLfloat materialAmbient[4] = { 0.5, 0.5, 0.5, 0.0 };
	GLfloat materialDiffuse[4] = { 0.8, 0.4, 0.4, 0.0 }; // zmena barvy osvetleni -> napr. cervena 1,0,0,0
	GLfloat materialShine[4] = { 0.8, 0.4, 0.4, 0.0 };
	GLfloat materialShineFactor[4] = { 0.0 }; // 30.0
	GLfloat lightPosition[4] = { 1.0, 1.0, 1.0, 0.0 }; 

	// nastavení OKNA aplikace
	Window Aplikace(VideoMode(800, 600), "Projekt Marching Cubes");
	Event Udalosti;
	// nastavení OpenGL
	glMatrixMode(GL_PROJECTION); // vykreslovací matice
	glOrtho(0, 800, 0, 800, -200, 200); // // promitani - vykreslení v x, y a z
	glScalef(1, -1, 1); // zvětšení
	glTranslatef(256, -512, 0); // iniciační pozice

	// nastavení OSVĚTLENÍ
	// stínování
	glShadeModel(GL_FLAT); // misto GL_SMOOTH -> GL_FLAT vidím pásy čtyřúhelníků (cvičení osvětlení)
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// nastavení materiálu - pro přední část 
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialShine);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShineFactor);
	// umístění světla
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	while (Aplikace.isOpen())
	{
		while (Aplikace.pollEvent(Udalosti))
		{
			switch (Udalosti.type)
			{
			// ukončení aplikace
			case Event::Closed:
				Aplikace.close();
				break;
			// při změně okna
			case Event::Resized:
				nastaveni.zmenaOkna(Udalosti.size.width, Udalosti.size.height); 
				break;
			// pokud se daná klávesa bude držet stisknutá
			case Event::KeyPressed:
				// switch stisknuté klávesy
				switch (Udalosti.key.code)
				{
				// ukončí se aplikace
				case Keyboard::Escape:
					Aplikace.close();
					cout << "Stiskli jste ESC." << endl;
					break;
				// přechod do fullscreen
				case Keyboard::W:
					cout << "Stiskli jste W." << endl;
					if (isFull) {
						// Přepni na původní velikost
						Aplikace.create(VideoMode(800, 600), "Projekt Marching Cubes", Style::Default);
						nastaveni.obnov();
						isFull = false; // nastaveno na false pro změnu režimu nazpět
						cout << "Prepnuto z Fullscreenu do defaultu." << endl;
						break;
					}
					else {
						// Přepni do fullscreen
						Aplikace.create(VideoMode(800, 600), "Projekt Marching Cubes", Style::Fullscreen);
						nastaveni.obnov();
						isFull = true; // nastaveno na true pro změnu režimu nazpět
						cout << "Prepnuto do Fullscreen." << endl;
						break;
					}
					break;
				// přiblížení
				case Keyboard::A:
					zoom = zoom + 0.1;
					cout << "Priblizeni objektu o 0.1." << endl;
					break;
				// oddálení
				case Keyboard::S:
					zoom = zoom - 0.1; 
					cout << "Oddaleni objektu o 0.1." << endl;
					break;
				// rotace ve všech úhlech
				case Keyboard::F:
					angle1 = angle1 + 5;
					angle2 = angle2 + 5;
					angle3 = angle3 + 5;
					cout << "Rotace ve vsech osach o +5 stupnu." << endl;
					break;
				// rotace ve všech úhlech v opačném směru
				case Keyboard::G:
					angle1 = angle1 - 5;
					angle2 = angle2 - 5;
					angle3 = angle3 - 5;
					cout << "Rotace ve vsech osach o -5 stupnu." << endl;
					break;
				// rotace v úhlu odpovídající ose x
				case Keyboard::H:
					angle1 = angle1 + 5;
					cout << "Rotace v ose x o +5 stupnu." << endl;
					break;
				// rotace v úhlu odpovídající ose x v opačném směru
				case Keyboard::J:
					angle1 = angle1 - 5;
					cout << "Rotace v ose x o -5 stupnu." << endl;
					break;
				// rotace v úhlu odpovídající ose y
				case Keyboard::K:
					angle2 = angle2 + 5;
					cout << "Rotace v ose y o +5 stupnu." << endl;
					break;
				// rotace v úhlu odpovídající ose y v opačném směru
				case Keyboard::L:
					angle2 = angle2 - 5;
					cout << "Rotace v ose y o -5 stupnu." << endl;
					break;
				// rotace v úhlu odpovídající ose z
				case Keyboard::Y:
					angle3 = angle3 + 5;
					cout << "Rotace v ose z o +5 stupnu." << endl;
					break;
				// rotace v úhlu odpovídající ose z v opačném směru
				case Keyboard::X:
					angle3 = angle3 - 5;
					cout << "Rotace v ose z o -5 stupnu." << endl;
					break;
				// posun nahoru
				case Keyboard::C: 
					translateX = translateX - 5;
					cout << "Posunuti obrazku nahoru o 5 pixelu." << endl;
					break;
				// posun dolů
				case Keyboard::V:
					translateX = translateX + 5;
					cout << "Posunuti obrazku dolu o 5 pixelu." << endl;
					break;
				// posun doleva
				case Keyboard::B:
					translateY = translateY - 5;
					cout << "Posunuti obrazku doleva o 5 pixelu." << endl;
					break;
				// posun doprava
				case Keyboard::N:
					translateY = translateY + 5;
					cout << "Posunuti obrazku doprava o 5 pixelu." << endl;
					break;
				// zapnutí světla
				case Keyboard::M: 
					if (isLight)
					{
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						cout << "Svetlo se vypnulo" << endl;
						isLight = false; // nastaveno na false pro změnu režimu nazpět
						break;
					}
					else
					{
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
						cout << "Svetlo se zapnulo" << endl;
						isLight = true; // nastaveno na true pro změnu režimu nazpět
						break;
					}
				// přepnutí na defaultní hodnoty, všechny rotace, translace a přiblížení se vynulují
				case Keyboard::D: 
					// zapsání do defaultních promenných
					angle1Default = angle1;
					angle2Default = angle2;
					angle3Default = angle3;
					translateXDefault = translateX;
					translateYDefault = translateY;
					// které jsou odečtené od aktualní hodnoty proměnné
					angle1 = angle1 - angle1Default;
					angle2 = angle2 - angle2Default;
					angle3 = angle3 - angle3Default;
					translateX = translateX - translateXDefault;
					translateY = translateY - translateYDefault;
					// a nastavení zoomu na hodnotu 1
					zoom = 1.0;
					cout << "Defaultni nastaveni." << endl;
					break;
				}
			}
		}
		// vykreslení SCÉNY - přenos hodnot
		nastaveni.zmenaCele(angle1, angle2, angle3, translateX, translateY, zoom);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // vymazání bufferu
		glBegin(GL_TRIANGLES); // vykreslení trojuhelníku
		glLoadIdentity(); //aktualizace sceny

		// promenne pro výpočet normál
		float vector1X, vector1Y, vector1Z, vector2X, vector2Y, vector2Z, nn, nx, ny, nz;
		for (int m = 0; m < xvert.size(); m = m + 1) 
		{
			//vykreslení normál
			glNormal3f(xnorm, ynorm, znorm);
			//vykreslení vrcholu
			glVertex3f(xvert.at(m), yvert.at(m), zvert.at(m));
		}
		//stanoveni normal pro 3 body, proto i+=3
		for (int i = 0; i < xvert.size(); i += 3) {

			vector1X = xvert[i] - xvert[i + 1];
			vector1Y = yvert[i] - yvert[i + 1];
			vector1Z = zvert[i] - zvert[i + 1];

			vector2X = xvert[i + 2] - xvert[i];
			vector2Y = yvert[i + 2] - yvert[i];
			vector2Z = zvert[i + 2] - zvert[i];

			nx = (vector1Y * vector2Z) - (vector2Y * vector1Z);
			ny = (vector1Z * vector2X) - (vector1X * vector2Z);
			nz = (vector1X * vector2Y) - (vector1Y * vector2X);
			//stanovení délky normály
			nn = sqrt((nx * nx) + (ny * ny) + (nz * nz));
			//normalizace
			xnorm = nx / nn;
			ynorm = ny / nn;
			znorm = nz / nn;
		}
		glEnd();
		glFlush();
		Aplikace.display();
	}
	return 0;
}