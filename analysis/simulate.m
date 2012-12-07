% Function to simulate the aircraft flying over the pipeline
% @Return: costs is a 4x1 vector containing all the individual cost terms
%          costs(1,1) - aircraft energy cost
%          costs(2,1) - total time cost
%          costs(3,1) - cyber utilization cost
%          costs(4,1) - cyber entropy/information cost

function [costs] = simulate(aircraft_vel, cyber_rate)

global min_vel max_vel min_cyber_rate max_cyber_rate;
persistent max_energy max_entropy max_time;
% some constants
end_pos = 2000; % end position in meters
start_pos = 0;
total_dist = end_pos - start_pos;
cyber_period = 1/cyber_rate;

% assign persistent variables
if(isempty(max_energy))
    max_energy = getMaxEnergy(total_dist,min_vel,max_vel);
    max_entropy = getMaxEntropy(total_dist,min_cyber_rate,max_vel);
    max_time = total_dist/min_vel;
end

cur_pos = start_pos + aircraft_vel*cyber_period;
prev_pos = start_pos;
aircraft_energy = 0; % in joules
time = 0; % in seconds
%scaled_total_time = min_vel/aircraft_vel; % total time scaled by max total time
cyber_util = 0;
entropy = 0;
while cur_pos <= end_pos
    del_dist = (cur_pos - prev_pos)/total_dist;
    
    % calculate the costs
    aircraft_energy = aircraft_energy + cyber_period/max_energy*getPower(aircraft_vel);
    time = time + cyber_period/max_time;
    cyber_util = cyber_util + del_dist*getUtilization(cyber_rate);
    entropy = entropy + aircraft_vel*cyber_period/max_entropy*getEntropy(cur_pos,prev_pos);

    % update our position
    prev_pos = cur_pos;
    cur_pos = cur_pos + aircraft_vel * cyber_period;
end;

costs = [aircraft_energy;
         time;
         cyber_util;
         entropy];

end