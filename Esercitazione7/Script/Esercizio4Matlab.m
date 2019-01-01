%P0=(2.5,-5) P1=(9,9), P2=(19,-2.5), P3=(22.5,7.5)
clear;clc;
P = [2.5,-5;9,9;19,-2.5;22.5,7.5];
xP = P(:,1);
yP = P(:,2);
%Disegno i punti
plot(xP,yP,'or');
hold on 
%Disegno il poligono
plot(xP,yP,'-r');
%Curva di Bézier
B1 = de_Casteljau(P,0.01);
xB1 = B1(:,1);
yB1 = B1(:,2);
%Disegno la curva
plot(xB1,yB1,'b');
%P3 = (19,-6)
%Lo sostituisco
P(4,:) = [19,-6];
xP = P(:,1);
yP = P(:,2);
%Disegno i punti
plot(xP,yP,'or');
hold on 
%Disegno il poligono
plot(xP,yP,'-g');
%Curva di Bézier
B2 = de_Casteljau(P,0.01);
xB2 = B2(:,1);
yB2 = B2(:,2);
%Disegno la curva
plot(xB2,yB2,'y');
title('Esercizio 4 - Curve di Bézier ')
%La 1 è quella di defaul
figure(2)

%Calcolo i polinomi di Bernestei
h = 0.01;
pB = polinomiBernstein(P,h);

pB0y = pB(:,1);
pB1y = pB(:,2);
pB2y = pB(:,3);
pB3y = pB(:,4);

%Intervalo delle x
pBx = 0:h:1;

%Disegno primo polinomio
plot(pBx,pB0y,'-b');
hold on
%Disegno secondo polinomio
plot(pBx,pB1y,'-g');
%Disegno terzo polinomio
plot(pBx,pB2y,'-y');
%Disegno quarto polinomio
plot(pBx,pB3y,'-m');
title('Esercizio 4 - Funzioni di base');


