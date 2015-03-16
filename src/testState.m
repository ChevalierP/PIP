State.setSpace(1:5, 6:10, 11:15);
s0 = State(2, 2, [2 2 2]);
i0 = State.toIndex(s0);
disp(i0);
s1 = State.fromIndex(i0);
disp([s1.v_i s1.theta_i s1.d_i(1) s1.d_i(2) s1.d_i(3)]);

i1 = State.toIndex(s1);
disp(i1);

sr = State.fromIndex(2478);
ir = State.toIndex(sr);
disp([2478 ir]);

ra = State('random');
disp([ra.v_i ra.theta_i ra.d_i(1) ra.d_i(2) ra.d_i(3)]);