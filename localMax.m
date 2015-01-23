function [ value, ind ] = localMax( Q, ligne )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

    ind = 1;
    value = Q(ligne,1);
    for i = 1:size(Q)
        if(Q(ligne,i)>value)
            ind = i;
            value = Q(ligne,i);
        end
    end;


end

