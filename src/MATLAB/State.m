% Espace d'�tat
classdef State < handle
    properties(SetAccess = private)
        v_i
        theta_i
        d_i

        v
        theta
        d
        
        index
        threshold = 1;
    end
    
    methods(Static)
        function setSpace(v, theta, d)  % d�finition de l'espace d'�tat
            State.speedSpace(v);        % d�finition de l'espace de la vitesse
            State.directionSpace(theta);% d�finition de l'espace de la direction
            State.distanceSpace(d);     % d�finition de l'espace des distances des capteurs
        end
        
        % Les fonctions *Space permettent de d�finir l'espace pour une
        % variable donn�e en passant l'espace en param�tre et retourne
        % l'espace si aucun param�tre n'est pr�cis�
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
            index = s.v_i + card_v*(s.theta_i - 1) + card_v*card_theta*(s.d_i(1) - 1)...
                    + card_v*card_theta*card_d*(s.d_i(2) - 1)...
                    + card_v*card_theta*card_d^2*(s.d_i(3) - 1);
        end
        
        function s = fromIndex(i)
            card_v = length(State.speedSpace);
            card_theta = length(State.directionSpace);
            card_d = length(State.distanceSpace);
            card = card_v;
            v_i = mod(i, card); %#ok<*PROP>
            if v_i == 0, v_i = card; end;
            ii = i - v_i;
            card = card*card_theta;
            theta_i = mod(ii, card)/card_v + 1;
            d_i = zeros(3, 1);
            ii = ii - card_v*(theta_i - 1);
            card = card*card_d;
            d_i(1) = mod(ii, card)/(card_v*card_theta) + 1;
            ii = ii - card_v*card_theta*(d_i(1) - 1);
            card = card*card_d;
            d_i(2) = mod(ii, card)/card*card_d + 1;
            ii = ii - card_v*card_theta*card_d*(d_i(2) - 1);
            d_i(3) = ii/card + 1;
            s = State(v_i, theta_i, d_i);
        end
    end
    
    methods
        function o = State(v_i, theta_i, d_i)
            o.v_i = v_i;
            o.theta_i = theta_i;
            if strcmp(o.v_i, 'random'), o.v_i = randi(length(State.speedSpace)); end
            if strcmp(o.theta_i, 'random'), o.theta_i = randi(length(State.directionSpace)); end
            o.d_i = d_i;
            o.v = State.speedSpace(o.v_i);
            o.theta = State.directionSpace(o.theta_i);
            N = length(o.d_i);
            o.d = zeros(N, 1);
            for i = 1:N
                o.d(i) = State.distanceSpace(d_i(i));
            end
            o.index = State.toIndex(o);
        end
        
        function b = isOutside(o)
            b = o.d(1) == -1 || o.d(2) == -1 || o.d(3) == -1;
        end
    end
    
end

