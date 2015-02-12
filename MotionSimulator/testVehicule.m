clear all
close all

%% Parcours d'un cercle à vitesse variable
% Véhicule en (1, 0) orienté selon y+
veh = Vehicule([1 0 pi/2]);
vitesse = abs(cos(1:200))';
theta = -pi/6*ones(200,1);
veh.addCommand([vitesse theta]);
veh.sim();
figure('name', 'Parcours d''un cercle à vitesse variable');
plot(veh.getX(), veh.getY());
axis equal

%% Parcours d'un circuit rectangulaire à vitesse constante
veh2 = Vehicule([1 0 pi/2]);
virage = [(0:9)'/10; ones(5,1); 1-(0:9)'/10]*(-pi/6);
theta = [zeros(25,1); virage; zeros(25,1); virage; zeros(25,1); virage; zeros(25,1); virage; 0];
veh2.addCommand([ones(201,1)*2.063, theta]);
veh2.sim();
figure('name', 'Parcours d''un circuit rectangulaire');
plot(veh2.getX(), veh2.getY());
axis equal