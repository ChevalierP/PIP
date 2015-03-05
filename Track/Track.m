classdef Track < handle
    properties
        width = 5;
        polygon
    end
    
    methods
        % predef : 'line', 'circle', 'turn'
        % arg :
        %  line : length
        function o = Track(predef, varargin)
            switch predef
                case 'line'
                    o.genLine(varargin{1});
            end
        end
        
        function genLine(o, length)
            o.polygon = [0 -o.width/2; 0 o.width/2; length o.width/2; length -o.width/2]; 
        end
        
        function plot(o)
            drawPolygon(o.polygon);
        end
        
        function d = getDistanceToBorders(o, veh)
            d = zeros(length(veh.sensors));
            for s = veh.sensors
                line = [veh.position(1:2) s.getGlobalDirection()];
                int = intersectLinePolygon(line, o.polygon);
                
                for i = 1:dim(int, 1)
                    int_dir = int(i,:) - veh.position(1:2);
                end
            end
        end
    end
end

