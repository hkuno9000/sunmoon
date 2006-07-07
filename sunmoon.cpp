/**@file sunmoon - ‘¾—zAŒ‚Ì‚“xŒvZ */
#include <iostream>
#include "planets.h"
#include "acoord.h"

using namespace std;
using namespace util;
using namespace astro;

//------------------------------------------------------------------------
int main()
{
    Planets pl;
    Jday dd(0);
    char buf[256];
    int y, m=1, d=0;
    fputs("input y.m.d [TDT 0h]\n",stderr);
    while (gets(buf)) {
        if (sscanf(buf, "%d.%d.%d.%d", &y, &m, &d) < 3)
            continue;
        dd.setGdate(y, m, d);
        AstroTime atime(dd, -31-32.184);
//        pl.calc(atime);
    }
    return EXIT_SUCCESS;
}
/*
1991.5.24
SUN:  L=62.35109377 R=1.01256784
MOON: L=191.20472784 B=-5.06906236 P=0.93588661
conv: l=321.23875670 b=-6.99444806 r=0.40761082
conv: l=184.61075748 b=3.22869300 r=0.72038002
conv: l=148.90626257 b=1.82425842 r=1.66473231
conv: l=137.74552429 b=0.79180431 r=5.32806686
conv: l=301.54231414 b=-0.34483677 r=9.97172726
conv: l=281.34882372 b=-0.35490606 r=19.47043026
conv: l=285.10627005 b=0.79262538 r=30.19843774
conv: l=229.09017978 b=15.10813030 r=29.66418493
*/
//. planets.cpp - ends.
