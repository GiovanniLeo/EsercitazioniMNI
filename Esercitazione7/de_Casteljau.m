function [B] = de_Casteljau(P,h)
%Input:
% - P = Vettore dei punti di controllo (Matrice Nx2, con N numero dei punti)
% - h = Passo con cui avanzare il valore di t nell' intervallo [0; 1]
%
%Output:
% - B = Vettore dei punti giacenti sulla curva di Bèzier C(t), cioè
%      B=[C(0), C(h), C(2h), C(3h),..., C(Nh)] (con Nh<=1)

if h < 0 || h > 1
    printf('Il valore di h deve essere compreso tra 0 e 1.\n');
    return
end

%inizializzazioni
t = 0:h:1;
N = length(t)-1;
n = size(P,1); %numero dei punti di controllo
dim=size(P,2);
B=zeros(N+1,dim);
B(1,:) = P(1,:); %La curva passa sempre per il primo punto di controllo
B(N+1,:) = P(n,:); %La curva passa sempre per l' ultimo punto di controllo

for i=2:N
    B(i,:) = de_Casteljau_single_point(P, n, t(i));
end