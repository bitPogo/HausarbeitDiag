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
Painter.stroke();
Painter.closePath();
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
//G
//2.1795532e1, 8.3117523
/*Painter.moveTo( 217.95532, 83.117523 );
Painter.arc(217.95532, 83.117523, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "G",
    217.95532,
    83.117523
);
Painter.moveTo( 175, 126.17615 );
/*
var w = [ 5000*1798.5498, 5000*-1798.5498 ]
var PP = [ 217.95532+w[0], 83.117523+w[1] ]
Painter.lineTo( PP[0], PP[1])*/
//1.7987675e5, -1.7984675
/*Painter.lineTo(1798767.5, -1798767.5);
//1.0345764e1 1.9771851e1
//2.4654236e1 5.4633789
//M'
Painter.moveTo( 246.54236, 54.6337891 );
Painter.arc(246.54236, 54.6337891, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "M'",
    246.54236,
    54.6337891
);
    //, 1.0117615 )
//L
//1.3204453e1, 8.3117523
Painter.moveTo( 132.04453, 83.117523 );
Painter.arc( 132.04453, 83.117523, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "L",
    132.04453,
    83.117523
);

//M
//2.5047913e1, 5.8800201
/*Painter.moveTo( 250.47913, 58.800201 );
Painter.arc( 250.47913, 58.800201, 2, 0, Math.PI * 2, true);
Painter.strokeText(
    "M",
    250.47913,
    58.800201
);*/

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

Painter.stroke();
