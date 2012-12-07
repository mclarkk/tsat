% optimize the cost functions
clear all;close all;clc;
% fmincon attempt
global min_vel max_vel min_cyber_rate max_cyber_rate;
min_vel = 9;
max_vel = 17;
min_cyber_rate = 3;
max_cyber_rate = 20;

% [vel, cyber_rate]
x0 = [10; 18]; % got this initial guess
% set lower bounds
lb = [min_vel;min_cyber_rate];
ub = [max_vel;max_cyber_rate];
options = optimset('Display','iter');
[x,fval] = fmincon(@evalCost,x0,[],[],[],[],lb,ub,[],options)