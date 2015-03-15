classdef State < handle
    properties(GetAccess = private)
        v_i
        theta_i
        d_i
    end
    properties(SetAccess = private)
        v
        theta
        d
        threshold = 1;
    end
    
    methods(Static)
        function setSpace(v, theta, d)  % définition de l'espace d'état
            State.speedSpace(v);        % définition de l'espace de la vitesse
            State.directionSpace(theta);% définirion de l'espace de la direction
            State.distanceSpace(d);     % définirion de l'espace des distances des capteurs
        end
        
        % Les fonctions *Space permettent de définir l'espace pour une
        % variable donnée en passant l'espace en paramètre et retourne
        % l'espace si aucun paramètre n'est précisé
        function space = speedSpace(v)
            persistent speed;
            if nargin == 0, space = speed;
            elseif length(v) > 1, speed = v;
            else space = speed(v);
            end
        end
        
        function space = directionSpace(theta)
            persistent direction;
            if nargin == 0, space = direction;
            elseif length(theta) > 1, direction = theta;
            else space = direction(theta);
            end
        end
        
        function space = distanceSpace(d)
            persistent distance;
            if nargin == 0, space = distance;
            elseif length(d) > 1; distance = d;
            else space = distance(d);
            end
        end
        
        function index = toIndex(s)
            card_v = length(State.speedSpace);
            card_theta = length(State.directionSpace);
            card_d = length(State.distanceSpace);
            index = s.v + card_v*(s.theta - 1) + card_v*card_theta*(s.d(1) - 1)...
                    + card_v*card_theta*card_d*(s.d(2) - 1)...
                    + card_v*card_theta*card_d^2*(s.d(3) - 1);
        end
        
        function s = fromIndex(i)
            card_v = length(State.speedSpace);
            card_theta = length(State.directionSpace);
            card_d = length(State.distanceSpace);
            v_i = mod(i, card_v); %#ok<*PROP>
            ii = i - v_i;
            theta_i = mod(ii/card_v, card_theta) + 1;
            d_i = zeros(3, 1);
            ii = ii - card_v*(theta_i - 1);
            d_i(1) = mod(ii, card_d)/(card_v*card_theta) + 1;
            ii = ii - card_v*card_theta*(d_i(1) - 1);
            d_i(2) = mod(ii/(card_v*card_theta*card_d), card_d) + 1;
            ii = ii - card_v*card_theta*card_d*(d_i(2) - 1);
            d_i(3) = ii/(card_v*card_theta*card_d^2) + 1;
            s = State(v_i, theta_i, d_i);
        end
    end
    
    methods
        function o = State(v_i, theta_i, d_i)
            if strcmp(v_i, 'random')
%                 V = 0:5:25;
%                 Theta = -50:10:50;
%                 D = -1:1;
%                 v_i = randi(length(V));
%                 theta_i = randi(length(Theta));
%                 d_i = randi(length(D), 3, 1);
%                 o.index = v_i*(length(V) - 1) + theta_i*(length(Theta) - 1) + prod(d_i);
%                 o.v = V(v_i);
%                 o.theta = Theta(theta_i);
%                 o.d = D(d_i);
            else
                o.v_i = v_i;
                o.theta_i = theta_i;
                o.d_i = d_i;
                o.v = State.speedSpace(v_i);
                o.theta = State.directionSpace(theta_i);
                N = length(d_i);
                o.d = zeros(N, 1);
                for i = 1:N
                    o.d(i) = d_i(i);
                end
%                 for i = 1:N
%                     if d(i) > o.threshold
%                         o.d(i) = 1;
%                     elseif d(i) > 0
%                         o.d(i) = 0;
%                     else
%                         o.d(i) = -1;
%                     end
%                 end
            end
        end
        
        function b = isOutside(o)
            b = o.d(1) == -1 || o.d(2) == -1 || o.d(3) == -1;
        end
    end
    
end

