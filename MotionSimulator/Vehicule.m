%% Classe permettant de simuler la position du centre de gravité
% d'un véhicule à partir d'une commande en vitesse et en angle de
% braquage des roues
classdef Vehicule < handle
    properties
        step = 1;           % nombre d'étape simulées
        time_step = 0.1;    % pas de temps entre deux étapes
        position            % vecteur de position [x, y, alpha]
                            % avec alpha l'angle formé par l'axe
                            % longitudinal du véhicule et les abscisses
        command = [0 0];    % vitesse et angle de braquage des roues
        l = 1;              % distance entre le centre de gravité et
                            % le train arrière selon l'axe longitudinal
        sensors
    end
    
    methods
        % Constructeur avec pos la position initiale
        function o = Vehicule(pos)
            o.position = pos;
            o.sensors = [Sensor(o,0) Sensor(o,pi/2) Sensor(o,-pi/2)];
        end
        
        function addCommand(o, command)
            o.command = vertcat(o.command, command);
        end
        
        function sim(o)
            step_count = size(o.command, 1) - size(o.position, 1);
            for x=1:step_count, o.oneStepSim(); end
        end
        
        function oneStepSim(o)
            % solveur différentiel utilisant Runge-Kutta aux ordres 4 et 5
            [~,x] = ode45(@(t,x) o.dpdt(t,x), [o.step o.step + 1]*o.time_step, o.position(end,:));
            o.position = [o.position; x(end,:)];
            o.step = o.step + 1;
        end
        
        function x = getX(o), x = o.position(:,1); end
        function y = getY(o), y = o.position(:,2); end
        
        % dérivée temporelle du vecteur position
        function d = dpdt(o,t,pos)
            v = o.command(floor(t/o.time_step),1);
            theta = o.command(floor(t/o.time_step),2);
            d = [v*cos(pos(3) - theta);
                 v*sin(pos(3) - theta);
                 -v*sin(theta)/o.l];
        end
        
        function alpha = getAlpha(o)
            alpha = o.position(3);
        end
    end
end