% Politique de récompense
classdef Reward < handle
    methods(Static)
        function r = value(from, to)
            if to.isOutside()
                r = -100;
            else
                r = to.v;
            end
        end
    end
end

