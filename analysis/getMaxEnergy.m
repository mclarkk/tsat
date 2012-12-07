% Function to get the maximum amount of energy consumed by TableSat for
% any length mission
% NOTE: this assumes TableSat is rotating at a constant angular velocity
function [max_energy] = getMaxEnergy(dist,max_vel)

% analyze max velocity case
max_power = getPower(max_vel);
max_vel_time = dist/max_vel;
max_vel_energy = max_vel_power*max_vel_time;
% analyze min velocity case
min_vel_power = getPower(min_vel);
min_vel_time = dist/min_vel;
min_vel_energy = min_vel_power*min_vel_time;

max_energy = max(max_vel_energy,min_vel_energy);

end