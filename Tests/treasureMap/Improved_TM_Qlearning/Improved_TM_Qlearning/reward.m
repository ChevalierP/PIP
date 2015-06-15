clc
close all
clear all

explo = csvread('recompense_exploration.csv');
combi = csvread('recompense_combinaison.csv');

plot(explo(:,2),explo(:,1),'r', 'LineWidth', 2)
hold on
plot(combi(:,2),combi(:,1),'g', 'LineWidth', 2)