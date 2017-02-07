s = [2 2 2 2 2 1 1 1 1];
t = [1 6 3 4 5 10 7 8 9];
G = graph(s, t);

L = laplacian(G);
L

D = diag(degree(G));

% normalized_laplacian NL.
DD = D^-0.5;

NL = DD * L * DD;

[e, v] = eig(full(L));

v