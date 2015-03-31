clear all
close all
addpath('geom');

track = Track('line', 20);
veh = Vehicule([2.5 20 pi/4]);
sensor_distance = track.getDistanceToBorders(veh);
disp(sensor_distance);
disp(track.isInside(veh));

circle = Track('circle', [0 0], 5);
%circle.plot();
veh2 = Vehicule([0 5 0]);
sensor_distance = circle.getDistanceToBorders(veh2);
disp(sensor_distance);
disp(circle.isInside(veh2));

turn = Track('turn', 20, 10);
turn.plot();
axis equal