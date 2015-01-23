function [ value ] = Max( Q )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

    value = Q(1,1);
    for i = 1:size(Q)
        for j = 1:size(Q)
            if(Q(i,j)>value)
                value = Q(i,j);
            end;
        end;
    end;

end

