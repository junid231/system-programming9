#include "coroutine.h"
#include "cube.h"

static int diroffset[4] = { -3, +1, +3, -1 };

static int dirfaces[6][4] =
{
    {  4, 1, 5, 3 },
    { 10, 2,23, 0 },
    { 16, 3,17, 1 },
    { 22, 0,11, 2 },
    { 14,19, 0, 9 },
    {  0, 7,14,21 },
};

static int rotmat[4][9] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
    { 6, 3, 0, 7, 4, 1, 8, 5, 2 },
    { 8, 7, 6, 5, 4, 3, 2, 1, 0 },
    { 2, 5, 8, 1, 4, 7, 0, 3, 6 },
};

/* ========== Global Variables (Just used in this file) ========== */

static Color new[6][9];
static Color curr[6][9];

/* =============================================================== */

Color GetColor(int face, int lednum) {
    return new[face][lednum];
}

void SetColor(int face, int lednum, Color newcolor) {
    new[face][lednum] = newcolor;
}

// dir: up 0, right 1, down 2, left 3
Color GetDirLED(int face, int lednum, int *newface, int *newlednum, int dir) {
    *newlednum = lednum + diroffset[dir];

    // out of face
    if (*newlednum < 0 || *newlednum >= 9 || (dir % 2 == 1 && (*newlednum / 3 != lednum / 3)))
    {
        *newface = dirfaces[face][dir] % 6;
        *newlednum = (*newlednum - 3 * diroffset[dir] + 9) % 9;

        // clockwise 90 deg rotation repitition
        int rotamount = dirfaces[face][dir] / 6;
        *newlednum = rotmat[rotamount][*newlednum];
    }
    // not out of face
    else *newface = face;

    return new[*newface][*newlednum];
}

Color GetDirLEDwithDist(int face, int lednum, int *newface, int *newlednum, int dir, int dist) {
    int distleft = dist;
    Color temp = new[face][lednum];

    while(distleft != 0)
    {
        temp = GetDirLED(face, lednum, newface, newlednum, dir);
        distleft = distleft > 0 ? distleft - 1 : distleft + 1;
    }

    return temp;
}

Color GetOppositeLED(int face, int lednum, int *newface, int *newlednum) {
    Color ret = new[face][lednum];
    if      (lednum  < 3) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 0, 4);
    else if (lednum == 3) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 3, 4);
    else if (lednum == 4) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 0, 6);
    else if (lednum == 5) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 1, 4);
    else if (lednum  < 9) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 2, 4);
    return ret;
}

Color MakeColor(int r, int g, int b) {
    Color color = {r, g, b};
    return color;
}

Color MakeRandomColor() {
    Color color;
    color.r = 50 + rand() % 206;
    color.g = 50 + rand() % 206;
    color.b = 50 + rand() % 206;
    return color;
}

void FadeColor(Coroutine* coroutine) {
    BEGIN_COROUTINE(coroutine);

    coroutine->processms = 0;
    coroutine->startcolor = curr[coroutine->face][coroutine->lednum];

    // run process
    while (coroutine->processms < coroutine->durationms)
    {
        curr[coroutine->face][coroutine->lednum].r = (int)(coroutine->startcolor.r + ((float)coroutine->processms/coroutine->durationms) * (new[coroutine->face][coroutine->lednum].r - coroutine->startcolor.r));
        curr[coroutine->face][coroutine->lednum].g = (int)(coroutine->startcolor.g + ((float)coroutine->processms/coroutine->durationms) * (new[coroutine->face][coroutine->lednum].g - coroutine->startcolor.g));
        curr[coroutine->face][coroutine->lednum].b = (int)(coroutine->startcolor.b + ((float)coroutine->processms/coroutine->durationms) * (new[coroutine->face][coroutine->lednum].b - coroutine->startcolor.b));
        coroutine->processms += deltaTime;
        YIELD(coroutine);
    }

    END_COROUTINE(coroutine);
}

void ChangeColorImm() {
    for(int face=0; face<6; face++) {
        for(int lednum=0; lednum<9; lednum++) {
            curr[face][lednum].r = new[face][lednum].r;
            curr[face][lednum].g = new[face][lednum].g;
            curr[face][lednum].b = new[face][lednum].b;
        }
    }
}

void CubeInit() {
    // Cube Color Initiate
    memset(new, 0, sizeof(Color) * 6*9);
    memset(curr, 0, sizeof(Color) * 6*9);
}

Color GetCurrColor(int face, int lednum) {
    return curr[face][lednum];
}