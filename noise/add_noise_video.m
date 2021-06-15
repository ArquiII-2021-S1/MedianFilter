clc; clear;

% Objetos que contienen la informacion de los videos
VA = VideoReader('source.mp4'); % Comando para cargar un video

% fr = VA.NumberOfFrames; % Numero de Marcos
m = VA.Height; % Numero de filas de cada marco
n = VA.Width; % Numero de columnas de cada marco

fr = 250;
Y = uint8(zeros(m, n, 1, fr)); % Video avion

% Leer los videos y guardar cada uno de los marcos
disp('Processing...');
for k = 1:fr
  frame = readFrame(VA);
  frame = frame(:,:,1);
  frame = imnoise(frame,'salt & pepper',0.1);
  Y(:,:,:,k) = frame;
end

disp('Saving file...');
% Crear el nuevo video
video = VideoWriter('video.mp4', 'MPEG-4'); % Crear el video nuevo (vacio)
open(video);
for i = 1:fr
  writeVideo(video, Y(:, :, :, i)); % Agregar cada marco del video
end
close(video);
disp('Finished');
