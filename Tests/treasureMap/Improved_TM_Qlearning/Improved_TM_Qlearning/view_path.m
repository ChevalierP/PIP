close all
clear all
%%Ce script trace sur la carte la politique otpimale obtenue

M = csvread('M.csv');%%choisir la matrice à importer
%%on récupère les log du programme
explo = csvread('log_exploration.csv');
combi = csvread('log_combinaison.csv');

x = zeros(38,3);
y = zeros(38,3);
for i=1:38
       x(i,1) = explo(i,2) + 1;
       y(i,1) = combi(i,2) + 1;
       x(i,2) = explo(i,1) + 1;
       y(i,2) = combi(i,1) + 1;
       x(i,3) = M(x(i,2) , x(i,1)) ;
       y(i,3) = M(y(i,2) , y(i,1)) ;
  
end

surf(M);
hold on
plot3(x(:,1),x(:,2), x(:,3),'Color', [0.5 0 0.5], 'LineWidth', 4)
plot3(y(:,1),y(:,2), y(:,3),'Color', [0.2 0 0.2], 'LineWidth', 4)
axis off;