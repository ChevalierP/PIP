clear all
close all
clc

% d_i = 0 si d <  1
%     = 1 si d >= 1
%     = -1 si 
% theta = -50:10:50 °
% v = 0:5:25 m/s
Q = zeros(1782);
gamma = 0.9;
track = Track('line', 20);
veh = Vehicule([0 0 0]);
S = State(0, 0, track.getDistanceToBorders(veh));

for i = 1:100000
    N = State('random');
    Q(S.index, N.index) = Reward.value(S, N) + gamma * max(Q(N.index,:));
    S = N;
end
