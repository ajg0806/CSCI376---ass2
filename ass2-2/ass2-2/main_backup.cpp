#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "kernel.cl"
#define KERNEL_FUNC "matvec_mult"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <ctype.h>
#include <sys/types.h>
using namespace std;

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

vector<char> getPlaintext() {
	vector<char> str;
	ifstream ifs;
	ifs.open("plaintext");
	char c;
	while (!ifs.eof())
	{
		ifs.get(c);

		if (!ifs.eof())
			str.push_back(c);
	}
	return str;
}

vector<char> formatPlainText(vector<char> str) {
	vector<char> fmtted;
	
	for (int i = 0; i < str.size(); i++) {
		char c = str[i];
		if ((c <= 'z' && c >= 'a') ||(c <= 'Z' && c >= 'A'))
			fmtted.push_back(tolower(c));
	}
	return fmtted;
}

void loadArrays(char* array1) {
	srand(time(NULL));
	for (int i = 0; i < 8; i++)
		array1[i] = (char)((rand() % 26)+97);
}

char shift_character(char c, int offset) {
	char tmp;
	if (isupper(c)) {
		tmp = (char)(c - offset);
		return tolower(tmp);
	}

	tmp = (char)(c + offset);
	return toupper(tmp);

}

vector<char> shift_text(vector<char> text, int offset) {
	vector<char> shifted_text;
	for (int i = 0; i < text.size(); i++)
		shifted_text.push_back(shift_character(text[i], offset));

	return shifted_text;
}

int main() {
	vector<char> str = getPlaintext();
	str = formatPlainText(str);

	for (int i = 0; i < str.size() / 10; i++)
		cout << str[i];
	cout << endl;
	str = shift_text(str, 3);
	for (int i = 0; i < str.size() / 10; i++)
		cout << str[i];
	cout << endl;
	str = shift_text(str, 3);
	for (int i = 0; i < str.size() / 10; i++)
		cout << str[i];
	cout << endl;


	/* Wait until a key is pressed before exiting */
	getchar();

	return 0;
}
