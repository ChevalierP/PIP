classdef Track < handle
    properties
        width = 5;
        polygon
    end
    
    methods
        % predef : 'line', 'circle', 'turn'
        % arg :
        %  line : length
        %  circle : center, radius
        %  turn : length_x, length_y
        function o = Track(predef, varargin)
            switch predef
                case 'line'
                    o.genLine(varargin{1});
                    
                case 'circle'
                    o.genCircle(varargin{1}, varargin{2});
                    
                case 'turn'
                    o.genTurn(varargin{1}, varargin{2});
            end
        end
        
        function genLine(o, length)
            o.polygon = [0 -o.width/2; 0 o.width/2; length o.width/2; length -o.width/2]; 
        end
        
        function genCircle(o, center, radius)
            ext = circleToPolygon([center radius + o.width/2], 64);
            int = flipud(circleToPolygon([center radius - o.width/2], 64));
            o.polygon = [ext ; int ; radius + o.width/2 0];
        end
        
        function genTurn(o, lengthx, lengthy)
            o.polygon = [0 -o.width/2; 0 o.width/2; lengthx - o.width/2 o.width/2;
                lengthx - o.width/2 lengthy + o.width/2; lengthx + o.width/2 lengthy + o.width/2;
                lengthx + o.width/2 -o.width/2]; 
        end
        
        function plot(o)
            drawPolygon(o.polygon);
        end
        
        function sensor_distance = getDistanceToBorders(o, veh)
            sensor_distance = zeros(length(veh.sensors), 1);
            for s = 1:length(veh.sensors)
                sensor = veh.sensors(s);
                sensor_vec = sensor.getGlobalDirection(); % vecteur direction du capteur
                line = [veh.getXY() sensor_vec];
                int = intersectLinePolygon(line, o.polygon);
                
                min_distance = inf;
                for i = 1:size(int, 1)
                    int_vec = int(i,:) - veh.getXY(); % vecteur direction du point d'intersection
                    distance = norm(int_vec);
                    if dot(sensor_vec, int_vec) > 0 && distance < min_distance
                        min_distance = distance;
                    end
                end
                sensor_distance(s) = min_distance;
            end
        end
        
        function isin = isInside(o, veh)
            xy = veh.getXY();
            isin = inpolygon(xy(1), xy(2), o.polygon(:,1), o.polygon(:,2));
        end
    end
end

