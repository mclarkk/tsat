% function to get information per radian of TableSat
% the information here is the number of orange (white when segmented) pixels
% obtained in one mission.
% The current setup has TableSat rotating within a cylindrical wall of
% paper. The amount of black is pi radians, and the amount of orange is pi
% radians.
% NOTE: Fortunately, we can assume that the vertical sweep of the camera is not
% beyond the wall of paper.

function [infoPerRadian] = getInformation(angular_vel, cyber_rate)

persistent gamma Ptot thetaOrange;
if(isempty(gamma))
    gamma = 13/84*2*pi; % horizontal sweep (in radians) of camera footprint
    Ptot = 640*480; % total number of pixels
    thetaOrange = pi; % total amount of orange paper
end

% first get the total number of frames taken per length of orange paper
%F = floor(cyber_rate*thetaOrange/angular_vel);
F = cyber_rate*thetaOrange/angular_vel;
% now determine the total distance swept by the camera in F frames (should
% be larger than thetaOrange)
%thetaF = gamma + F*angular_vel/cyber_rate;
thetaF = gamma + thetaOrange;
% calculate the approximate number of pixels that were orange by using the
% proportion of total swept distance to thetaOrange (note that whatever was
% swept beyond thetaOrange is assumed to be black). If >1 that means I
% didn't sweep as much area as there was orange and therefore all the
% pixels I saw were orange.
proportion = thetaOrange/thetaF; 
% foo = floor(proportion*F*Ptot/(2*pi));
% infoPerRadian = exp(-4*foo);
infoPerRadian = 1/(proportion*F*Ptot/(2*pi));

end