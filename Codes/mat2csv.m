clear;
clc;

% FileData = load('arc130.mat');
% S = full(FileData.Problem.A);
% csvwrite('arc130.csv', S);

FileData = load('GD96_d.mat');
S = full(FileData.Problem.A);
n = 229;    %non-zero
size = 180;
x = zeros(n,1);
y = zeros(n,1);
cnt = 1;
for i=1:size
    for j=1:size
        if S(i,j)==1
            x(cnt) = i;
            y(cnt) = j;
            cnt = cnt + 1;
        end
    end
    
end
csvwrite('GD96_d.csv', [x y]);