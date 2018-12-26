function [C] = de_Casteljau_single_point(P, n, t)
%Input:
% - P = Vettore dei punti di controllo (Matrice nx2, con n numero dei punti di controllo)
% - n = numero dei punti di controllo
% - t = Parametro reale appartenente all' intervallo [0;1]
%
%Output:
% - C = Vettore rappresentante il punto sulla curva di Bézier c(t)

for k = 1:n-1
    for i = 1:n-k
        P(i,:) = (1 - t)*P(i,:) + t*P(i+1,:);
    end
end
C = P(1,:);
