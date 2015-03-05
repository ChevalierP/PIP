clear all
close all

poly = [0 0;10 0;10 10;0 10];
linee = [5 5 1 0];


plot(poly(:,1), poly(:,2));
hold on
plot([5 5],[6 5]);

intersectLinePolygon(linee, poly)