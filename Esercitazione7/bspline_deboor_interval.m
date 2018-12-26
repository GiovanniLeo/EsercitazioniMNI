%Utilizzato per calcolare il knot-span in cui si trova il knot che viene
%aggiunto, ricorda che questo algoritmo di De Boor si basa sul knot
%insertion.
function ix = bspline_deboor_interval(u,t)
% Index of knot in knot sequence not less than the value of u.
% If knot has multiplicity greater than 1, the highest index is returned.

i = bsxfun(@ge, u, t) & bsxfun(@lt, u, [t(2:end) 2*t(end)]);  % indicator of knot interval in which u is
[row,col] = find(i); %trova gli indici degli elementi diversi da 0 all'interno di una matrice
[row,ind] = sort(row);  %#ok<ASGLU> % restore original order of data points
ix = col(ind);