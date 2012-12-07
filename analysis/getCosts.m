% get the costs
% this is the same as the simulate() function, but it assumes that velocity
% and cyber rate will be constant over the flight, whereas simulate() can
% handle varying values

function [costs] = getCosts(aircraft_vel, cyber_rate)

global min_vel max_vel min_cyber_rate max_cyber_rate;
persistent max_energy max_entropy max_time;
% some constants
end_pos = 2000; % end position in meters
start_pos = 0;
total_dist = end_pos - start_pos;
cyber_period = 1/cyber_rate;
del_dist = aircraft_vel*cyber_period;

% assign persistent variables
if(isempty(max_energy))
    max_energy = getMaxEnergy(total_dist,min_vel,max_vel);
    max_entropy = getMaxEntropy(total_dist,min_cyber_rate,max_vel);
    max_time = total_dist/min_vel;
end

total_time = total_dist/aircraft_vel;
% compute the costs (don't need a while loop)
aircraft_energy = total_time * getPower(aircraft_vel)/max_energy;
%time = total_time * cyber_period/max_time;
time = total_time/max_time;
%cyber_util = del_dist*getUtilization(cyber_rate);
cyber_util = cyber_rate/max_cyber_rate;
entropy = total_dist * getEntropy(del_dist,0)/max_entropy;

costs = [aircraft_energy;
         time;
         cyber_util;
         entropy];

end