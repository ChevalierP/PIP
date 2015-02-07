close all
clear all

x0 = [1 0 pi/2];
l = 1;
theta = -pi/6;
v = 2*pi*l/sin(-theta);
delta_t = 0.01;
t = 0:delta_t:0.8;

[t,x] = ode45(@(t,x) f(t,x,delta_t,v,theta,l), t, x0);

plot(x(:,1), x(:,2));
axis equal;