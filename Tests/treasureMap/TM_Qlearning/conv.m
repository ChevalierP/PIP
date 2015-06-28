clear all;
close all;
clc;

a = csvread('C:\Users\ChevalierP\Documents\GitHub\PIP\Tests\treasureMap\TM_Qlearning\TM_Qlearning\log.csv');
plot(1:size(a,1), a(:,1), 'r')
hold on;
plot(1:size(a,1), a(:,2), 'b')
title('reward = f( iteration number)');
xlabel('iteration number');
ylabel('reward');

