/*Copyright 2016-2017 Siborgium
You're free to copy, replace, share, use, modify and do other actions with this program under rules of GNU GPL*/
#pragma once
#include<windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<random>

enum buttonlist { ID_BUTTON_GEN = 01470, ID_BUTTON_PLA = 01471, ID_BUTTON_RIGHT = 01472, ID_BUTTON_LEFT = 01473 };
UINT current_hero = 0;

namespace sbl {
	namespace err {
		struct no_such_file;
		UINT ErrorCount = 0;
	}
}

struct sbl::err::no_such_file {
	no_such_file() { sbl::err::ErrorCount++; }
	~no_such_file() {};
};

std::mt19937::result_type getRandom(std::mt19937::result_type from, std::mt19937::result_type to) {
	static std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(from, to);
	return dist(rng);
}


std::string loadStringFromFile(const char* fname) {
	std::string s;
	try {
		std::ifstream file; file.open(fname, std::ios::in); if (!file) { throw sbl::err::no_such_file(); }
		std::vector<std::string> v;
		while (!file.eof() || !file.badbit) {
			std::string c;
			std::getline(file, c);
			v.push_back(c);
		}
		s = v[getRandom(0, v.size() - 1)];
		v.~vector();
	}
	catch (sbl::err::no_such_file) { MessageBoxA(hWnd, "No source file, try to download program again and make sure you download everything \"as is\"", fname, MB_ICONERROR); abort(); }
	return s;
}

std::vector<std::string> loadinterface(const char* fname) {
	std::vector<std::string> v;
	try {
		std::ifstream file; file.open(fname, std::ios::in); if (!file) { throw sbl::err::no_such_file(); }
		while (!file.eof() || !file.badbit) {
			std::string c;
			std::getline(file, c);
			v.push_back(c);
		}
	}
	catch (sbl::err::no_such_file) { MessageBoxA(hWnd, "No source file, try to download program again and make sure you download everything \"as is\"", fname, MB_ICONERROR); abort(); }
	return v;
}

std::string loadfontname(const char* fname){
	std::string s;
	try {
		std::ifstream file; file.open(fname, std::ios::in); if (!file) { throw sbl::err::no_such_file(); }
		std::getline(file, s);
	}
	catch (sbl::err::no_such_file) { MessageBoxA(hWnd, ("No source file, try to download program again and make sure you download everything \"as is\""), fname, MB_ICONERROR); abort(); }
	return s;
}

struct hero {
	std::string s[6];
	hero() {
		s[0] = "Still working on it, sorry. Try to use your fantasy ^^";
		s[1] = loadStringFromFile("res\\races");
		s[2] = loadStringFromFile("res\\beliefs");
		s[3] = loadStringFromFile("res\\legacy");
		s[4] = loadStringFromFile("res\\weapon");
		s[5] = loadStringFromFile("res\\patterns");
	}
	void draw(HDC hdc, HFONT hFont) {
		SelectObject(hdc, hFont);
		const uint16_t ten = 30;	uint16_t x = 220, y = 10;
		for (uint16_t i = 0; i < 6; i++) {
			TextOutA(hdc, x, y, s[i].c_str(), s[i].length()); y += ten;
		}
	}
	~hero() {};
};

std::vector<std::string> *religion;//массив религий
std::vector<std::string> *pattern;//массив мировоззрений
std::vector<std::string> *weapon;//массив с оружием
std::vector<std::string> *legacy;//массив с происхождениями
std::vector<std::string> *interfacev;//массив с интерфейсом
std::vector<hero> *database;

void initInterface() {
	*interfacev = loadinterface("tech\\interface");
	interfacev->shrink_to_fit();
}

int InterfaceOnScreen(HFONT hFont, HDC hdc){
	const int ten = 30;
	int x = 30, y = 10;
	SelectObject(hdc, hFont);
	for (int i = 0; i < interfacev->size(); i++) { TextOutA(hdc, x, y, interfacev->operator[](i).c_str(), interfacev->operator[](i).length()); y = y + ten; }
	TextOutA(hdc, 515, 400, "Siborgium", 9);
	return 0;
}

int ResultOnScreen(HDC hdc,HFONT hFont){
	int x = 220, y = 10;
	const int ten = 30;//шаг
	SelectObject(hdc, hFont);
	database->operator[](current_hero).draw(hdc ,hFont);
	return 0;
}

HFONT CreateDefaultFont(int height, const char* fontname) {
	return CreateFont(height, 0, 0, 0, 0, 0u, 0U, 0U, DEFAULT_CHARSET, 0U, 0U, 0U, 0U, TEXT(fontname));
}
