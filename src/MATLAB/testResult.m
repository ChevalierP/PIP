State.setSpace(linspace(0, 25, 6), (-50:10:50)*pi/180, 0:1);

track = Track('line', [0 0], 5);
veh = Vehicule([25 0 0]);
S = State.fromIndex(1);

stop = 0;
for i = 1:100
    d = track.getDistanceToBorders(veh);
    d_i = zeros(length(d), 1);
    for j = 1:length(d)
        if d(j) == Inf, stop = 1; break;
        elseif d(j) >= 1, d_i(j) = 2;
        else d_i(j) = 1;
        end
    end
    if stop, break; end
    S = State(S.v_i, S.theta_i, d_i);
    
    [~, I] = sort(Q(S.index,:), 'descend');
    for j = 1:length(I)
        s = State.fromIndex(I(j));
        if s.d_i == d_i, N = s; break; end
    end
    veh.addCommand([N.v N.theta]);
    veh.sim();
end

track.plot();
hold on
plot(veh.getX, veh.getY);