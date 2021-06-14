clc; clear; close all;
pkg load image;

% A = imread('edificio_china.png');
A = imread('beach.png')(:,:,1);
subplot(1,2,1);
A = im2double(A);
imshow(A);
title('Imagen original');

[m,n] = size(A);
Z = salt_and_pepper(m, n, 0.1);
M = A + Z;
subplot(1,2,2);
imshow(M);
title('Ruido sal y pimienta');

G = im2uint8(M);
imwrite(G, 'ruido.png');