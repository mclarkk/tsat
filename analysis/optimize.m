% optimize the cost functions
clear all;close all;clc;
% fmincon attempt
global  min_angular_vel max_angular_vel min_cyber_rate max_cyber_rate;
min_angular_vel = 0.95;
max_angular_vel = 5.52;
min_cyber_rate = 1;
max_cyber_rate = 8;

% [vel, cyber_rate]
x0 = [3; 2]; % got this initial guess
% set lower bounds
lb = [min_angular_vel;min_cyber_rate];
ub = [max_angular_vel;max_cyber_rate];
options = optimset('Display','iter');
[x,fval] = fmincon(@evalCost,x0,[],[],[],[],lb,ub,[],options)