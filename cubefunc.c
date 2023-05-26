#include "coroutine.h"
#include "cube.h"

float fadeduration = 0.5f;
float longpressduration = 2.0f;

int diroffset[4] = { -3, +1, +3, -1 };

int dirfaces[6][4] =
{
    {  4, 1, 5, 3 },
    { 10, 2,23, 0 },
    { 16, 3,17, 1 },
    { 22, 0,11, 2 },
    { 14,19, 0, 9 },
    {  0, 7,14,21 },
};

int rotmat[4][9] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
    { 6, 3, 0, 7, 4, 1, 8, 5, 2 },
    { 8, 7, 6, 5, 4, 3, 2, 1, 0 },
    { 2, 5, 8, 1, 4, 7, 0, 3, 6 },
};

/* ========== Global Variables (Just used in this file) ========== */

LED led[6][9];
LED currled[6][9];
LED prevled[6][9];

/* =============================================================== */

Color GetColor(int face, int lednum) {
    return led[face][lednum].color;
}

void SetColor(int face, int lednum, Color newcolor) {
    prevled[face][lednum].color.r = led[face][lednum].color.r;
    prevled[face][lednum].color.g = led[face][lednum].color.g;
    prevled[face][lednum].color.b = led[face][lednum].color.b;

    led[face][lednum].color.r = newcolor.r;
    led[face][lednum].color.g = newcolor.g;
    led[face][lednum].color.b = newcolor.b;
}

// dir: up 0, right 1, down 2, left 3
LED *GetDirLED(int face, int lednum, int dir)
{
    int newlednum;
    int newface;

    newlednum = lednum + diroffset[dir];

    // out of face
    if (newlednum < 0 || newlednum >= 9 || (dir % 2 == 1 && (newlednum / 3 != lednum / 3)))
    {
        newface = dirfaces[face][dir] % 6;
        newlednum = (newlednum - 3*diroffset[dir] + 9) % 9;

        // clockwise 90 deg rotation repitition
        int rotamount = dirfaces[face][dir] / 6;
        newlednum = rotmat[rotamount][newlednum];
    }
    // not out of face
    else newface = face;

    return (led[newface] + newlednum);
}

LED *GetDirLEDwithDist(int face, int lednum, int dir, int dist)
{
    int distleft = dist;
    LED *currLED = (led[face] + lednum);

    while(distleft != 0)
    {
        currLED = GetDirLED(face, lednum, dir);
        distleft = distleft > 0 ? distleft - 1 : distleft + 1;
    }

    return currLED;
}

LED *GetOppositeLED(int face, int lednum)
{
    LED *retLED = (led[face] + lednum);
    if      (lednum  < 3) retLED = GetDirLEDwithDist(face, lednum, 0, 4);
    else if (lednum == 3) retLED = GetDirLEDwithDist(face, lednum, 3, 4);
    else if (lednum == 4) retLED = GetDirLEDwithDist(face, lednum, 0, 6);
    else if (lednum == 5) retLED = GetDirLEDwithDist(face, lednum, 1, 4);
    else if (lednum  < 9) retLED = GetDirLEDwithDist(face, lednum, 2, 4);
    return retLED;
}

Color MakeRandomColor() {
    Color color;
    color.r = rand() % 256;
    color.g = rand() % 256;
    color.b = rand() % 256;
    return color;
}

Color MakeColor(int r, int g, int b) {
    Color color = {r, g, b};
    return color;
}

void ChangeColorImm() {
    for(int face=0; face<6; face++) {
        for(int lednum=0; lednum<9; lednum++) {
            currled[face][lednum].color.r = led[face][lednum].color.r;
            currled[face][lednum].color.g = led[face][lednum].color.g;
            currled[face][lednum].color.b = led[face][lednum].color.b;
        }
    }
}

void FadeColor(Coroutine* coroutine) {
    float step = deltaTime / fadeduration; // The amount of change to apply.
    float colorprocess = 0.0f;
    static int face;
    static int lednum;

    BEGIN_COROUTINE(coroutine);

    while (colorprocess < 1)
    {
        for(face=0; face<6; face++) {
            for(lednum=0; lednum<9; lednum++) {
                currled[face][lednum].color.r = (int)(prevled[face][lednum].color.r + colorprocess * (led[face][lednum].color.r - prevled[face][lednum].color.r));
                currled[face][lednum].color.g = (int)(prevled[face][lednum].color.g + colorprocess * (led[face][lednum].color.g - prevled[face][lednum].color.g));
                currled[face][lednum].color.b = (int)(prevled[face][lednum].color.b + colorprocess * (led[face][lednum].color.b - prevled[face][lednum].color.b));
                colorprocess += step;
                YIELD(coroutine);
            }
        }
    }

    END_COROUTINE();
}