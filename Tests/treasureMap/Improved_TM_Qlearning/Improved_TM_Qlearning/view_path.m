close all
clear all
M = csvread('M.csv');
explo = csvread('log_exploration.csv');
combi = csvread('log_combinaison.csv');
surf(M);
hold on
plot3(explo(:,2)+1,explo(:,1)+1,ones(38)*0.5, 'r')
plot3(combi(:,2)+1,combi(:,1)+1,ones(38)*0.5, 'g')