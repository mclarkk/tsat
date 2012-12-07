% function to get the maximum entropy
% the max entropy would be at the fastest speed and the slowest cyber rate

function [max_entropy] = getMaxEntropy(dist,min_cyber_rate,max_vel)

del_dist = max_vel/min_cyber_rate;
% get a slice of entropy
entropy_slice = getEntropy(del_dist,0);
max_entropy = entropy_slice*dist;

end