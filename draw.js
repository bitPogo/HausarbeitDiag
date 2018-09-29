var Canvas = document.getElementById( 'app' );
var Painter = Canvas.getContext( "2d" );
var BaseX = 20
var BaseY = 20
/*
Kantenlänge is:1.5000000e1
Kantenlänge/2 is:7.5000000
Point A is: (1.0000000e1, 1.0000000e1)
Point B is: (2.5000000e1, 1.0000000e1)
Point C is: (1.0000000e1, 2.5000000)
Point D is: (2.5000000e1, 2.5000000)
Point E is: (1.7500000e1, 1.0000000e1)
Point Z is: (1.7500000e1, 2.5000000)

Set HZ Punkte.
Eine Einheit ist: 7.6988816e-2
EH ist: 2.6176186
Point H is: (1.7500000e1, 1.2617615e1)
Point KH is: 6.0821152
Point K is: (1.7500000e1, 6.5354996)
Point OH is: 4.0034180
Point O is: (1.7500000e1, 8.6141968)
Point SH is: 8.8537140
Point S is: (1.7500000e1, 3.7639008)
Calculated G is: (1.6620850e1, 1.8635864e1)

 */

Painter.beginPath();

//AB
Painter.moveTo( 100, 100 );
Painter.arc(100, 100, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "A",
    100,
    110
);
Painter.moveTo( 100, 100 );
Painter.lineTo( 250, 100);
Painter.arc( 250, 100, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "B",
    250,
    110
);
//AC
Painter.moveTo( 100, 100 );
Painter.lineTo( 100, 25);
Painter.arc(100, 25, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "C",
    90,
    35
);
//BD
Painter.moveTo( 250, 100);
Painter.lineTo( 250, 25);
Painter.arc( 250, 25, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "D",
    255,
    35
);
//CD
Painter.lineTo( 100, 25);
//H
Painter.moveTo( 175, 126.17615 );
Painter.lineTo( 175, 25);

//E
Painter.moveTo( 175, 100 );
Painter.arc(175, 100, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "E",
    180,
    100
);
//Z
Painter.moveTo( 175, 25 );
Painter.arc(175, 25, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "Z",
    180,
    20
);
//H
Painter.moveTo( 175, 126.17615 );
Painter.arc(175, 126.17615, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "H",
    180,
    126.17615
);
//K
Painter.moveTo( 175, 65.354996 );
Painter.arc(175, 65.354996, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "K",
    180,
    65.354996
);
//O
Painter.moveTo( 175, 86.141968 );
Painter.arc(175, 86.141968, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "O",
    180,
    86.141968
);

//S
Painter.moveTo( 175, 37.639008 );
Painter.arc(175, 37.639008, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "S",
    180,
    37.639008
);

//ZH
Painter.moveTo( 175, 126.17615 );
Painter.arc( 175, 126.17615, 101.17615, 0,  Math.PI * 2, true);
//OH
Painter.moveTo( 175, 126.17615 );
Painter.arc( 175, 126.17615, 40.034180, 0,  Math.PI * 2, true);
//KH
Painter.moveTo( 175, 126.17615 );
Painter.arc( 175, 126.17615, 60.821152, 0,  Math.PI * 2, true);
//SH
Painter.moveTo( 175, 126.17615 );
Painter.arc( 175, 126.17615, 88.537140, 0,  Math.PI * 2, true);


//Calculated G is: (2.2307037e1, 8.8914337)
Painter.moveTo( 223.07037, 88.914337 );
Painter.arc( 223.07037, 88.914337, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "G",
    223.07037,
    88.914337
);


//Gerade Verlängern
//HG'
//9.2889355e2, -8.9877637e2
Painter.moveTo(175, 126.17615)
Painter.lineTo( 1272.3657, -724.44824 )


//M
//Calculated M is: (2.5496521e1, 6.4191055)
Painter.moveTo( 254.96521, 64.191055 );
Painter.arc( 254.96521, 64.191055, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "M",
    254.96521,
    64.191055
);

//X
//Calculated X is: (2.0664124e1, 1.0164948e1)
Painter.moveTo( 206.64124, 101.64948 );
Painter.arc( 206.64124, 101.64948, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "X",
    206.64124,
    101.64948
);

//R
//Calculated R is: (2.4497589e1, 7.1934280)
Painter.moveTo( 244.97589, 71.934280 );
Painter.arc( 244.97589, 71.934280, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "R",
    244.97589,
    71.934280
);

//F
//Calculated F is: (2.3995728e1, 4.8605881)
Painter.moveTo( 239.95728, 48.605881 );
Painter.arc( 239.95728, 48.605881, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "F",
    239.95728,
    48.605881
);

//L
//Calculated L ist: (1.2692963e1, 8.8914337)
Painter.moveTo( 126.92963, 88.914337 );
Painter.arc( 126.92963, 88.9143371, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "L",
    126.92963,
    88.914337
);
//HL'
//Calculated L' ist: (-6.1535767e1, -4.8647095e1)
Painter.moveTo( 175, 126.17615)
Painter.lineTo( -615.35767, -486.47095 )

//N
//Calculated N ist: (9.5034790, 6.4191055)
Painter.moveTo( 95.034790, 64.191055 );
Painter.arc( 95.034790, 64.1910551, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "N",
    95.034790,
    64.191055
);

//P
//Calculated P ist: (1.4335876e1, 1.0164948e1)
Painter.moveTo( 143.35876, 101.64948 );
Painter.arc( 143.35876, 101.64948, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "P",
    143.35876,
    101.64948
);
//T
//Calculated T ist: (1.0502411e1, 7.1934280)
Painter.moveTo( 105.02411, 71.934280 );
Painter.arc( 105.02411, 71.934280, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "T",
    105.02411,
    71.934280
);
//U
//Calculated U ist: (1.1004272e1, 4.8605881)
Painter.moveTo( 110.04272, 48.605881 );
Painter.arc( 110.04272, 48.605881, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "U",
    110.04272,
    48.605881
);

//Parallelkreise
Painter.moveTo( 175, 126.17615 );
Painter.arc( 175, 126.17615, 88.537140, 0, Math.PI * 2, true);

Painter.stroke()
Painter.closePath();




//Glob
Painter.beginPath();
var AddY = 1000;
//AB
Painter.moveTo( 100, 100+AddY );
Painter.arc(100, 100+AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "A",
    100,
    110+AddY
);
Painter.moveTo( 100, 100+AddY );
Painter.lineTo( 250, 100+AddY );
Painter.arc( 250, 100+AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "B",
    250,
    110+AddY
);
//AC
Painter.moveTo( 100, 100+AddY );
//Painter.lineTo( 100, 25+AddY);
Painter.arc(100, 25+AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "C",
    90,
    35+AddY
);
//BD
Painter.moveTo( 250, 100+AddY);
//Painter.lineTo( 250, 25+AddY);
Painter.arc( 250, 25+AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "D",
    255,
    35+AddY
);
//CD
Painter.lineTo( 100, 25+AddY);
//E
Painter.moveTo( 175, 100+AddY );
Painter.arc(175, 100+AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "E",
    180,
    100+AddY
);
//Z
Painter.moveTo( 175, 25 +AddY);
Painter.arc(175, 25+AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "Z",
    180,
    20+AddY
);
//H
//Point H ist: (1.7500000e1, 3.8680573)
Painter.moveTo( 175, 38.680573 + AddY)
Painter.arc( 175, 38.680573 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "H",
    176,
    39.680573 + AddY
);

//G
//Point G ist: (1.7500000e1, 5.8541718)
Painter.moveTo( 175, 58.541718 + AddY)
Painter.arc( 175, 58.541718 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "G",
    176,
    59.541718 + AddY
);

//K
//Point K ist: (1.7500000e1, 9.1180573)
Painter.moveTo( 175, 91.180573 + AddY)
Painter.arc( 175, 91.180573 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "K",
    176,
    92.180573 + AddY
);

//L
//Point L ist: (1.7500000e1, 1.9020813e1)
Painter.moveTo( 175, 190.20813 + AddY)
Painter.arc( 175, 190.20813 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "L",
    176,
    191.20813 + AddY
);
Painter.lineTo( 175, 25 +AddY);

//Hilskreise

//HL
Painter.moveTo( 175, 190.20813 + AddY );
Painter.arc( 175, 190.20813 + AddY, 151.52771, 0, Math.PI * 2, true);
//GL
Painter.moveTo( 175, 190.20813 + AddY );
Painter.arc( 175, 190.20813 + AddY, 131.66672, 0, Math.PI * 2, true);
//ZL
Painter.moveTo( 175, 190.20813 + AddY );
Painter.arc( 175, 190.20813 + AddY, 165.20844, 0, Math.PI * 2, true);
//KL
Painter.moveTo( 175, 190.20813 + AddY );
Painter.arc( 175, 190.20813 + AddY, 99.027710, 0, Math.PI * 2, true);
//EA
Painter.moveTo( 175, 58.541718 + AddY );
Painter.arc( 175, 58.541718 + AddY, 165.20844, 0, Math.PI * 2, true);

//Limits
//P
//Point P ist: (1.3414673e1, 1.0000000e1)
Painter.moveTo( 134.14673, 100 + AddY)
Painter.arc( 134.14673, 100 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "P",
    135.14673,
    101 + AddY
);

//R
//Point R ist: (2.1585327e1, 1.0000000e1)
Painter.moveTo( 215.85327, 100 + AddY)
Painter.arc( 215.85327, 100 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "R",
    216.85327,
    101 + AddY
);

//X
//Point X ist: (1.0000000e1, 8.1990204)
Painter.moveTo( 100, 81.990204 + AddY)
Painter.arc( 100, 81.990204 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "X",
    101,
    82.990204 + AddY
);

//O
//Point O ist: (2.5000000e1, 8.1990204)
Painter.moveTo( 250, 81.990204 + AddY)
Painter.arc( 250, 81.990204 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "O",
    250,
    82.990204 + AddY
);

//M
//Point M ist: (1.0000000e1, 4.3004684)
Painter.moveTo( 100, 43.004684 + AddY)
Painter.arc( 100, 43.004684 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "M",
    101,
    43.004684 + AddY
);

//N
// Point N ist: (2.5000000e1, 4.3004684)
Painter.moveTo( 250, 43.004684 + AddY)
Painter.arc( 250, 43.004684 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "N",
    251,
    44.004684 + AddY
);

//Y
//2.0810028e1, 9.6876373
Painter.moveTo( 208.10028, 96.876373 + AddY )
Painter.arc( 208.10028, 96.876373 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "Y",
    209.10028,
    97.876373 + AddY
);

//V
//1.4189957e1, 9.6876373
Painter.moveTo( 141.89957, 96.876373 + AddY )
Painter.arc( 141.89957, 96.876373 + AddY , 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "V",
    142.89957,
    97.876373 + AddY
);

//T
//2.4066803e1, 7.6086426
Painter.moveTo( 240.66803, 76.086426 + AddY )
Painter.arc( 240.66803, 76.086426 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "T",
    241.66803,
    77.086426 + AddY
);

//I
//Point I ist: (2.4697510e1, 5.6865692)
Painter.moveTo( 246.97510, 56.865692 + AddY )
Painter.arc( 246.97510, 56.865692 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "I",
    247.97510,
    57.865692 + AddY
);

//S
//Point S ist: (2.4519531e1, 4.0654144)
Painter.moveTo( 245.19531, 40.654144 + AddY )
Painter.arc( 245.19531, 40.654144 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "S",
    246.19531,
    41.654144 + AddY
);

//U
//Point U ist: (1.0933197e1, 7.6086426)
Painter.moveTo( 109.33197,  76.086426 + AddY )
Painter.arc( 109.33197,  76.086426 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "U",
    110.33197,
    77.086426 + AddY
);
//J
//Point J ist: (1.0302490e1, 5.6865692)
Painter.moveTo( 103.02490, 56.865692 + AddY )
Painter.arc( 103.02490, 56.865692 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "J",
    104.02490,
    57.865692 + AddY
);

//F
//Point F ist: (1.0480484e1, 4.0654144)
Painter.moveTo( 104.80484, 40.654144 + AddY )
Painter.arc( 104.80484, 40.654144 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "F",
    104.80484,
    40.654144 + AddY
);


//Y'
//Point Y' ist: (1.0544083e1, 1.1972412e1)
Painter.moveTo( 105.44083, 119.72412 + AddY )
Painter.arc( 105.44083, 119.72412 + AddY, 1, 0, Math.PI * 2, true);
Painter.strokeText(
    "Y'",
    105.44083, 119.72412 + AddY
);





Painter.stroke()
Painter.closePath();
