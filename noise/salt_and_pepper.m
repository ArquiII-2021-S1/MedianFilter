function Z = salt_and_pepper(m, n, p)
  % Funcion para generar un ruido de sal y pimienta
  % m: numero de filas, double
  % n: numero de columnas, double
  % p: probabilidad de cambiar un pixel, double entre 0 y 1
  Z = zeros(m,n);
  p = p*100;
  x = randi([0, 100], [m, n]);
  for i = 1:m
    for j = 1:n
      if (x(i,j) < p)
        Z(i,j) = randi([-1, 1]);
      endif
    endfor
  endfor
endfunction