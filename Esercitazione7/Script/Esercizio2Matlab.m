%P0=(0,0) P1=(1,3), P2=(3,5), P3=(5,12) , P4=(7,4)
P = [0,0;1,3;3,5;5,12;7,4];
xP = P(:,1); %sto dindicando le x
yP = P(:,2); %sto dindicando le x
plot(xP,yP,'or');%Plot marker poligono di controllo
hold on;
plot(xP,yP,'r'); %Plot poligono di controllo
B = de_Casteljau(P,0.01);
xB = B(:,1);
yB = B(:,2);
plot(xB,yB,'b'); %plot curva

%Scalo la matrice B e ne faccio il plot
scaledB = B*0.5;
scaledXB= scaledB(:,1);
scaledYB = scaledB(:,2);
plot(scaledXB,scaledYB,'g'); %plot curva scalata

%scalo P
scaledP = P*0.5;
scaledXP = scaledP(:,1); %sto dindicando le x
scaledYP = scaledP(:,2); %sto dindicando le x
plot(scaledXP,scaledYP,'ok'); %Plot marker poligono di controllo
plot(scaledXP,scaledYP,'m'); %Plot  poligono di controllo
sB = de_Casteljau(scaledP,0.01);
sxB = sB(:,1);
syB = sB(:,2);
plot(sxB,syB,'k'); %Plot curva
title('Esercizio 2 - Curva di Bézier');
%controlo se la curva scalata è uguala alla curva che si viene a formare
%scalando P
checkEqual = isequal(scaledB,sB);

%Controllo se le curve sono uguali
if(checkEqual == 1)   
    disp('La curva scalata coincide con la curva che si ottiene scalando P.');
end