clc; clear;

% Objetos que contienen la informacion de los videos
VA = VideoReader('noisy_video.mp4'); % Comando para cargar un video

m = VA.Height; % Numero de filas de cada marco
n = VA.Width; % Numero de columnas de cada marco

% Crear folder donde guardar los frames del video
[~, ~, ~] = mkdir('video');

% Guardar cada uno de los marcos
k = 0;
while hasFrame(VA)
  frame = readFrame(VA); % Leer cada marco del video
  fname = sprintf('video/frame%d.png', k); % Nombre del archivo
  imwrite(frame,fname);
  k = k + 1;
end
