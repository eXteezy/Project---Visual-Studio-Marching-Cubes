#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>

using namespace std;

class nastavGL
{
public:
	void zmenaCele(int angle1, int angle2, int angle3, int translateX, int translateY, float zoom);
	void zmenaOkna(int sirka, int vyska);
	void obnov();
};

