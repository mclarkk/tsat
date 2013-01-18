% Function to get the maximum amount of energy consumed by TableSat for
% any "length" mission (where length is angle)
% NOTE: this assumes TableSat is rotating at a constant angular velocity
function [max_energy] = getMaxEnergy(dist,min_angular_vel,max_angular_vel)

% analyze max velocity case
max_vel_power = getPower(max_angular_vel);
max_vel_time = dist/max_angular_vel;
max_vel_energy = max_vel_power*max_vel_time;
% analyze min velocity case
min_vel_power = getPower(min_angular_vel);
min_vel_time = dist/min_angular_vel;
min_vel_energy = min_vel_power*min_vel_time;

% if(max_vel_energy>min_vel_energy)
%     disp 'Max energy from MAX angular velocity';
% else
%     disp 'Max energy from MIN angular velocity';
% end
max_energy = max(max_vel_energy,min_vel_energy);

end