% get the costs
% Assumes that velocity and cyber rate will be constant over the mission
% @Return: costs(1): tsat energy (Joules)
%          costs(2): time (seconds)
%          costs(3): cyber utilization
%          costs(4): information (info (pixels) per radian)

function [costs] = getCosts(tsat_angular_vel, cyber_rate)

global min_angular_vel max_angular_vel min_cyber_rate max_cyber_rate;
persistent max_energy max_information max_time;
% some constants
%total_dist = 4*pi;
total_dist = 2*pi;

% assign persistent variables
if(isempty(max_energy))
    max_energy = getMaxEnergy(total_dist,min_angular_vel,max_angular_vel);
    max_information = getMaxInformation(total_dist,min_cyber_rate,max_angular_vel);
    max_time = total_dist/min_angular_vel;
end 

total_time = total_dist/tsat_angular_vel;
% compute the costs
tsat_energy = total_time * getPower(tsat_angular_vel)/max_energy;
time = total_time/max_time;
cyber_util = cyber_rate/max_cyber_rate;
information = getInformation(tsat_angular_vel,cyber_rate)/(total_dist*max_information);
%information = getInformation(tsat_angular_vel,cyber_rate)/total_dist;

costs = [tsat_energy;
         time;
         cyber_util;
         information];

end