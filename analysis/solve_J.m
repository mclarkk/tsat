% try to solve this nasty problem
clear all;close all;clc;


syms v r;
syms w_i A_i b1 b2 b3 b4 n1 n2 n3 r_max v_min;

J = b1*n1*v^2 + b1*n2/v^2 + b2*v_min/v + b3*r/r_max + b4*n3*exp(w_i*v/(r*A_i));

J_by_r = diff(J,r)
J_by_v = diff(J,v)

%J_by_r = b3/r_max - w_i*v*b4*n3/A_i * exp(w_i*v/(r*A_i));
%J_by_v = 2*b1*n1*v-2*b1*n2/v^3-b2*v_min/v^2+w_i*b4*n3/(r*A_i)*exp(w_i*v/(r*A_i));

% S_by_r = solve(J_by_r,v)
% S_by_v = solve(J_by_v,r)

S = solve(J_by_r,J_by_v,v,r)
