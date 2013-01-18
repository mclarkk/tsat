% get un-normalized costs for a specific velocity and cyber rate

function [costs,total] = getUnnormalizedCosts(angular_vel,cyber_rate,weights)
D = 2*pi;
max_r = 8;

total_time = D/angular_vel
tsat_energy = total_time * getPower(angular_vel)
cyber_util = cyber_rate/max_r
info = getInformation(angular_vel, cyber_rate)/D

costs(1) = tsat_energy;
costs(2) = total_time;
costs(3) = cyber_util;
costs(4) = info;

% now get the normalized costs
global min_angular_vel max_angular_vel;
min_angular_vel = 0.95;
max_angular_vel = 5.52;
global max_cyber_rate min_cyber_rate; % max cyber rate to devote to image acquisition (Hz)
min_cyber_rate = 1;
max_cyber_rate = max_r;
costs = getCosts(angular_vel,cyber_rate);
tsat_energy = costs(1,1);
total_time = costs(2,1);
cyber_util = costs(3,1);
info = costs(4,1);
weights = weights/sum(weights);
ac_weight = weights(1);
time_weight = weights(2);
util_weight = weights(3);
ent_weight = weights(4);
total = ac_weight*tsat_energy + time_weight*total_time...
    + util_weight*cyber_util + ent_weight*info
end