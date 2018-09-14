#include <iostream>
using namespace std;

char *lookup_generate_01()
{
    char *l1 = new char[26];

    l1[0] = 'C';
    l1[1] = 'I';
    l1[2] = 'S';
    l1[3] = 'Q';
    l1[4] = 'V';
    l1[5] = 'N';
    l1[6] = 'F';
    l1[7] = 'O';
    l1[8] = 'W';
    l1[9] = 'A';
    l1[10] = 'X';
    l1[11] = 'M';
    l1[12] = 'T';
    l1[13] = 'G';
    l1[14] = 'U';
    l1[15] = 'H';
    l1[16] = 'P';
    l1[17] = 'B';
    l1[18] = 'K';
    l1[19] = 'L';
    l1[20] = 'R';
    l1[21] = 'E';
    l1[22] = 'Y';
    l1[23] = 'D';
    l1[24] = 'Z';
    l1[25] = 'J';

    return l1;
}

char *lookup_generate_02()
{
    char *l2 = new char[26];
    l2[0] = 'j';
    l2[1] = 'r';
    l2[2] = 'a';
    l2[3] = 'x';
    l2[4] = 'v';
    l2[5] = 'g';
    l2[6] = 'n';
    l2[7] = 'p';
    l2[8] = 'b';
    l2[9] = 'z';
    l2[10] = 's';
    l2[11] = 't';
    l2[12] = 'l';
    l2[13] = 'f';
    l2[14] = 'h';
    l2[15] = 'q';
    l2[16] = 'd';
    l2[17] = 'u';
    l2[18] = 'c';
    l2[19] = 'm';
    l2[20] = 'o';
    l2[21] = 'e';
    l2[22] = 'i';
    l2[23] = 'k';
    l2[24] = 'w';
    l2[25] = 'y';

    return l2;
}

int main()
{

    char *l1, *l2;

    l1 = lookup_generate_01();
    l2 = lookup_generate_02();

    for (int i = 0; i < 26; i++)
    {
        cout << char('A' + i) << "\t|\t" << l2[i] << endl;
    }
    cout << endl;

    return 0;
}