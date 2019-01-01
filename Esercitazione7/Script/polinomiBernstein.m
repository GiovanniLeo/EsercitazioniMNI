function [C] = polinomiBernstein(P,h)
%Input
%- P = punti
%- h = intervallo
%

% t è un intero
t = 0:h:1;
len = length(t);

for i = 1:len
    B0(i,:) = (1-t(i))^3;
    B1(i,:) = (3*t(i))*(1-t(i))^2;
    B2(i,:) = (3*t(i)^2)*(1-t(i));
    B3(i,:) = t(i)^3;
end
C = [B0,B1,B2,B3];