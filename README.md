# sunmoon

make time table of sunrise, sunset, moonrise, moonset and culmination.

supports and source codes at: [github](https://github.com/hkuno9000/sunmoon/)

## What to do

* calc altitude of Sun and Moon.
```
$ sunmoon -r
LT: +35d 10m 00s
LG: E136d 55m 00s
SEA: 0m
UTC: 2016-09-27T00:32:34
LOC: 2016-09-27 09:32:34 JST
LST: 10h 05m 11s
SUN-ALT: +42.12d
MOON-ALT: +65.57d
MOON-PHASE: +314.38d
```

* calc R.A, Dec, azimuth and altitude of Sun, Moon and planets.
```
$ sunmoon -r -p
LT: +35d 10m 00s
LG: E136d 55m 00s
SEA: 0m
UTC: 2016-09-27T00:34:18
LOC: 2016-09-27 09:34:18 JST
LST: 10h 06m 55s
SUN-ALT: +42.37d
MOON-ALT: +65.41d
MOON-PHASE: +314.39d
SUN     : RADEC[12h 15m 55s(183.98d)  -01d 43m 24s(-01.72d)], J2000[12h 15m 04s(183.77d)  -01d 37m 52s(-01.63d)], AZALT[313.80d(SE)  +42.37d]
MOON    : RADEC[09h 21m 38s(140.41d)  +12d 47m 53s(+12.80d)], J2000[09h 20m 43s(140.18d)  +12d 52m 15s(+12.87d)], AZALT[027.38d(SSW)  +65.41d]
MERCURY : RADEC[11h 12m 12s(168.05d)  +05d 58m 06s(+05.97d)], J2000[11h 11m 20s(167.83d)  +06d 03m 34s(+06.06d)], AZALT[328.98d(SSE)  +57.17d]
VENUS   : RADEC[14h 07m 34s(211.89d)  -12d 55m 16s(-12.92d)], J2000[14h 06m 40s(211.67d)  -12d 50m 37s(-12.84d)], AZALT[298.66d(ESE)  +15.58d]
MARS    : RADEC[17h 59m 06s(269.77d)  -25d 53m 23s(-25.89d)], J2000[17h 58m 04s(269.52d)  -25d 53m 30s(-25.89d)], AZALT[278.12d(East)  -36.63d]
JUPITER : RADEC[12h 15m 36s(183.90d)  -00d 29m 53s(-00.50d)], J2000[12h 14m 45s(183.69d)  -00d 24m 21s(-00.41d)], AZALT[312.89d(SE)  +43.41d]
SATURN  : RADEC[16h 40m 01s(250.00d)  -20d 41m 50s(-20.70d)], J2000[16h 39m 02s(249.76d)  -20d 40m 03s(-20.67d)], AZALT[282.86d(ESE)  -18.24d]
URANUS  : RADEC[01h 26m 58s(021.74d)  +08d 26m 11s(+08.44d)], J2000[01h 26m 06s(021.52d)  +08d 21m 05s(+08.35d)], AZALT[122.67d(WNW)  -25.76d]
NEPTUNE : RADEC[22h 47m 26s(341.86d)  -08d 38m 10s(-08.64d)], J2000[22h 46m 34s(341.64d)  -08d 43m 29s(-08.72d)], AZALT[158.34d(NNW)  -61.83d]
PLUTO   : RADEC[19h 04m 24s(286.10d)  -21d 24m 31s(-21.41d)], J2000[19h 03m 25s(285.85d)  -21d 26m 11s(-21.44d)], AZALT[263.43d(East)  -47.89d]
```

* make 3 days time table of sunrise, sunset, moonrise, moonset and culmination.
```
$ sunmoon -r table=3
LT: +35d 10m 00s
LG: E136d 55m 00s
SEA: 0m
UTC: 2016-09-27T00:40:46
LOC: 2016-09-27 09:40:46 JST
LST: 10h 13m 24s
SUN-ALT: +43.31d
MOON-ALT: +64.77d
MOON-PHASE: +314.42d
SUN-CULM: 2016-09-27 11:43:17
MOON-SET: 2016-09-27 15:35:29
SUN-SET: 2016-09-27 17:42:36
MOON-RISE: 2016-09-28 02:53:43
SUN-RISE: 2016-09-28 05:44:11
MOON-CULM: 2016-09-28 09:36:06
SUN-CULM: 2016-09-28 11:42:57
MOON-SET: 2016-09-28 16:12:41
SUN-SET: 2016-09-28 17:41:10
MOON-RISE: 2016-09-29 03:51:03
SUN-RISE: 2016-09-29 05:44:57
MOON-CULM: 2016-09-29 10:22:07
SUN-CULM: 2016-09-29 11:42:37
MOON-SET: 2016-09-29 16:47:12
SUN-SET: 2016-09-29 17:39:45
MOON-RISE: 2016-09-30 04:47:21
SUN-RISE: 2016-09-30 05:45:43
```

## Usage
```
usage: sunmoon [-h?rpj] [lt=<LT>] [lg=<LG>] [sea=<SEA>] [utc=<UTC>] [repeat=<N>,<STEP>] [leap=<LEAP>] [table=<DAYS>]
  version 2018.5
  -h -?: this help
  -r   : add refraction to ALT
  -p   : print RADEC,J2000,AZALT of Sun, Moon and planets
  -j   : print J2000 only for -p
  LT   : latidute.  default is NAGOYA '35d10m00s'
  LG   : longitude. default is NAGOYA '136d55m00s'
  SEA  : sea level altitude[m]. default is 0
  UTC  : ISO 8601 time format '2014-12-31T23:59:59'. default is current time
  N    : repeat count. default is 1
  STEP : repeat step days,hours,minutes or seconds. default is '1day'
  DAYS : time table days of sunrise, sunset, moonrise, moonset and culmination. default is 0
  LEAP : TAI-UTC leap seconds. default is +37(2017 January 1). see https://hpiers.obspm.fr/iers/bul/bulc/bulletinc.dat
```

## Install
```
git clone https://github.com/hkuno9000/sunmoon.git
cd sunmoon
make
# install to /usr/local/bin
sudo make install
# install to /usr/bin
# sudo make install PREFIX=/usr
```

## History

#### Jul.2015
* move to GitHub from my local svn-repos r371.
```
git svn clone --trunk=trunk/src --authors-file=users.txt --prefix=svn/ http://c196/svn/remotecam sunmoon-git
git remote add origin https://hkuno9000@github.com/hkuno9000/sunmoon.git
git push -u -f origin master
```

#### Jul.2006
* start the project.

## Source
The formulas used in this program were obtained from the textbooks below.
- 天体の位置計算（増補版）, 長沢　工, 株式会社地人書館, ISBN4-8052-0225-4 C3044

## License
This project is licensed under the terms of the MIT license.
