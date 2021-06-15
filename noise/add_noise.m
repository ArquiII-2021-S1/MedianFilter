clc; clear; close all;
pkg load image;

% A = imread('edificio_china.png');
A = imread('video/frame50.png')(:,:,1);
subplot(1,2,1);
imshow(A);
title('Imagen original');

M = imnoise(A,'salt & pepper', 0.1);
subplot(1,2,2);
imshow(M);
title('Ruido sal y pimienta');

imwrite(M, 'ruido.png');