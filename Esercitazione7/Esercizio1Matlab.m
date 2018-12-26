%Punto 1 esercizio
P = [2.5,-5;9,9;19,-2.5;22.5,7.5]; 
xP = P(:,1); %sto dindicando le x
yP = P(:,2); %sto dindicando le y
plot(xP,yP,'or');
hold on;
plot(xP,yP,'r');
B = de_Casteljau(P,0.01);
xB = B(:,1);
yB = B(:,2);
plot(xB,yB,'b');
%Punto 2 esercizio
mP =  [2.5,-5;9,9;19,-6;22.5,7.5];
mxP = mP(:,1); %sto dindicando le x
myP = mP(:,2); %sto dindicando le x
plot(mxP,myP,'og');
hold on;
plot(mxP,myP,'g');
mB = de_Casteljau(mP,0.01);
mxB = mB(:,1);
myB = mB(:,2);
plot(mxB,myB,'k');
legend('P1','P2','P3');