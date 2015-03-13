classdef State < handle
    properties
        v
        theta
        d
        index
        threshold = 1;
    end
    
    methods
        function o = State(v, theta, d)
            if strcmp(v, 'random')
                V = 0:5:25;
                Theta = -50:10:50;
                D = -1:1;
                v_i = randi(length(V));
                theta_i = randi(length(Theta));
                d_i = randi(length(D), 3, 1);
                o.index = v_i*(length(V) - 1) + theta_i*(length(Theta) - 1) + prod(d_i);
                o.v = V(v_i);
                o.theta = Theta(theta_i);
                o.d = D(d_i);
            else
                o.v = v;
                o.theta = theta;
                N = length(d);
                o.d = zeros(N, 1);
                for i = 1:N
                    if d(i) > o.threshold
                        o.d(i) = 1;
                    elseif d(i) > 0
                        o.d(i) = 0;
                    else
                        o.d(i) = -1;
                    end
                end
            end
        end
        
        function b = isOutside(o)
            b = o.d(1) == -1 || o.d(2) == -1 || o.d(3) == -1;
        end
    end
    
end

