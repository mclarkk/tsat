% function to get entropy assuming a particular camera and a
% particular height above ground
% the entropy here is an exponential function of the percent 
% overlap in ground coverage between the current and previous images

% NOTE: y1,y2 are image coords, y1 to the right, y2 up
%       x1,x2 are world coords, x1 to the right, x2 up
% points are as [y1;y2] and [x1;x2]
% Assumptions: camera is always traveling in the x2 (also y2) direction
%              assume [0;0] is at center of the image
function [entropy] = getEntropy(cur_pos, prev_pos)

hag = 30; % height above ground in meters
% if we're at position 0 there's no overlap to calculate
if(cur_pos == 0) 
    entropy = calcEntropy(0);
else % calculate the overlap
    [ul_prev,ur_prev,ll_prev,lr_prev] = getCoords(prev_pos,hag);
    [ul_cur,ur_cur,ll_cur,lr_cur] = getCoords(cur_pos,hag);
    % assuming image is always a rectangle
    tot_image_area = (ur_cur(1)-ul_cur(1)) * (ur_cur(2)-lr_cur(2));
    % assuming image overlap is always a rectangle
    % and that camera never moves backward
    overlap = ul_prev(2) - ll_cur(2);
    if(overlap<0)
        overlap = 0;
    end
    overlap_area = (lr_cur(1)-ll_cur(1)) * overlap;
    entropy = calcEntropy(overlap_area/tot_image_area);
end

end

% omega is % overlap
function [ent] = calcEntropy(omega)

alpha = 4; % scaling in entropy
ent = exp(-alpha*omega);

end

% calculate the coordinates of the corners of the image on the ground
function [ul,ur,ll,lr] = getCoords(pos, hag)

persistent focal_length;
persistent Hdist Vdist Hdist_half Vdist_half;
persistent ul_c ur_c ll_c lr_c;
if(isempty(focal_length))
    focal_length = 0.0046; % meters
    % need ul, ur, ll, lr coordinates in meters
    % using 3.61 mm H x 2.72 mm V
    Hdist = 0.00361; % horizontal distance of imaging plane in meters
    Vdist = 0.00272; % vertical distance of imaging plane in meters
    Hdist_half = Hdist/2;
    Vdist_half = Vdist/2;
    % get image corners in the camera coordinate system
    ul_c = [-Hdist_half; Vdist_half];
    ur_c = [Hdist_half; Vdist_half];
    ll_c = [-Hdist_half; -Vdist_half];
    lr_c = [Hdist_half; -Vdist_half];
end

% since i assume that camera is always traveling in the x2 (also y2)
% direction, only need to care about changes in the x2 direction
% get image corners in the ground coordinate system
scaling = hag/focal_length;
offset = [0;pos];
ul = ul_c.*scaling + offset;
ur = ur_c.*scaling + offset;
ll = ll_c.*scaling + offset;
lr = lr_c.*scaling + offset;

end