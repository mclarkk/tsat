% function to act as a wrapper for the simulate function, and which
% will return the appropriate cost function

function [cost] = evalCost(params)

vel = params(1);
cyber_rate = params(2);

global ac_weight_Jp time_weight_Jp;
% set weights for Jp
ac_weight_Jp = 10;
time_weight_Jp = 10;

global util_weight_Jc ent_weight_Jc;
% set weights for Jc
util_weight_Jc = 10;
ent_weight_Jc = 10;

global ac_weight time_weight util_weight ent_weight;
% set weights for J
ac_weight = 10;
time_weight = 10;
util_weight = 10;
ent_weight = 10;

%costs = simulate(vel,cyber_rate);
costs = getCosts(vel,cyber_rate);
aircraft_energy = costs(1,1);
total_time = costs(2,1);
cyber_util = costs(3,1);
entropy = costs(4,1);
% calculate costs
% Jp
%J = ac_weight_Jp*aircraft_energy + time_weight_Jp*total_time;
% Jc
%J = util_weight_Jc*cyber_util + ent_weight_Jc*entropy;
J = ac_weight*aircraft_energy + time_weight*total_time...
    + util_weight*cyber_util + ent_weight*entropy;
cost = J;

end