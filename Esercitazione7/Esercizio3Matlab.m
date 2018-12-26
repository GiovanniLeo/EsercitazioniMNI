clear;clc;
u = [0,0,0,0,1,2,3,4,5,5,6,7]; %knot vector
P = [-2.5,-8;0,3;1,-2;5,-7.5;10,4;11,6;15,4;22.5,7.5]; %punti di controllo
xP = P(:,1);
yP = P(:,2);
plot(xP,yP,'o');
hold on;
plot(xP,yP,'r');

n = 4;
lP = transpose(P);
%Abbiamo fatto la trasposta perchè la funzione vuole che le x e le y
%siano sulle righe e non sulle colonne ovvero vuole dei vettori riga 
%Per capire basta guardare l'esempio  example_bsplinedeboor

C =  bspline_deboor(n,u,lP);
xC = C(1,:);
yC = C(2,:);
plot(xC,yC, 'g');

