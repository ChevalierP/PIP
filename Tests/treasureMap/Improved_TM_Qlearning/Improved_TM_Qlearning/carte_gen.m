clear all
clc
close all
M = randn(20);%%creation d'une carte aléatoire de dimension 20
M = max(zeros(size(M)), M);
for i=0:8 %%lissage de la carte
    M = ndnanfilter(M,'hamming',[2 2],1);
end;

%%coef. loggés dans M4.csv
csvwrite('M4.csv',M);

surf(M)