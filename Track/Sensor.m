classdef Sensor < handle
    properties
        vehicule
        angle
    end
    
    methods
        function o = Sensor(vehicule, angle)
            o.vehicule = vehicule;
            o.angle = angle;
        end
        
        function v = getGlobalDirection(o)
            v = [cos(o.angle + o.vehicule.getAlpha())
                 sin(o.angle + o.vehicule.getAlpha())];
        end
    end
end

