classdef C < handle
    methods
        function o = C(a)
            if a > 0, o = C(a-1); end
        end
    end
end