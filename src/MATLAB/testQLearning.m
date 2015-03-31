clear all
close all
clc
addpath('geom');

% d_i = 0 si d <  1
%     = 1 si d >= 1
% theta = -50:10:50 °
% v = 0:5:25 m/s
State.setSpace(0:5:25, (-50:10:50)*pi/180, 0:1);
Q = zeros(1782);
gamma = 0.9;

niter = 10000;
for iter = 1:niter
    track = Track('circle', [0 0], 5);
    veh = Vehicule([5 1 pi/2]);
    S = State.fromIndex(1);

    stop = 0;
    for i = 1:1000
        d = track.getDistanceToBorders(veh);
        d_i = zeros(length(d), 1);
        for j = 1:length(d)
            if d(j) == Inf, stop = 1; break;
            elseif d(j) >= 1, d_i(j) = 2;
            else d_i(j) = 1;
            end
        end
        if stop, break; end

        N = State('random', 'random', d_i);
        Q(S.index, N.index) = Reward.value(S, N) + gamma * max(Q(N.index,:));
        S = N;
        veh.addCommand([S.v S.theta]);
        veh.sim();
    end
    
    disp([floor(iter/niter*100) floor(i/10)]);
end
