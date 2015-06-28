clc
close all
clear all
%%ce script trace la valeur de la récompense sur le chemin otpiml en
%%fonction du nombre d'itérations effectuées.

explo = csvread('cost.csv');

plot(explo(:,1),'r', 'LineWidth', 2) %% exploration pure
hold on
plot(explo(:,2),'g', 'LineWidth', 2) %% combinaison
title('reward = f(iteration number)');
xlabel('iteration number');
ylabel('reward');