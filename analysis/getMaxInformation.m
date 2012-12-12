% function to get the maximum information
% the max information would be at the fastest speed and the slowest cyber rate

function [maxInfo] = getMaxInformation(dist,min_cyber_rate,max_angular_vel)

% get info per radian
infoPerRadian = getInformation(max_angular_vel,min_cyber_rate);
maxInfo = infoPerRadian*dist;

end