classdef Vehicule < handle
    properties
        step = 1;
        time_step = 0.1;
        position
        command = [0 0];
        l = 1;
    end
    
    methods
        function o = Vehicule(pos)
            o.position = pos;
        end
        
        function addCommand(o, command)
            o.command = vertcat(o.command, command);
        end
        
        function sim(o)
            step_count = size(o.command, 1) - size(o.position, 1);
            disp(((0:step_count - 1) + o.step)*o.time_step);
            [~,x] = ode45(@(t,x) o.dpdt(t,x), ((0:step_count - 1) + o.step)*o.time_step, o.position(end,:));
            o.position = vertcat(o.position, x(2:end,:));
        end
        
        function d = dpdt(o,t,pos)
            v = o.command(floor(t/o.time_step),1);
            theta = o.command(floor(t/o.time_step),2);
            d = [v*cos(pos(3) - theta);
                 v*sin(pos(3) - theta);
                 -v*sin(theta)/o.l];
            
        end
    end
end