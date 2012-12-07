% plot entropy
clear variables; close all;clc;
% some constants
min_vel = 7;
max_vel = 17;
v = min_vel:0.2:max_vel;
cyber_rate = 3;

end_pos = 2000; % end position in meters
start_pos = 0;
total_dist = end_pos - start_pos;
cyber_period = 1/cyber_rate;

% find the max entropy which would be fastest speed and slowest cyber rate
max_entropy = getMaxEntropy(total_dist,cyber_rate,max(v));

entropy = zeros(1,length(v));
for i=1:length(v)
    cur_pos = start_pos;
    prev_pos = start_pos;
    while cur_pos <= end_pos
        del_dist = (cur_pos - prev_pos)/total_dist;
        entropy(i) = entropy(i) + del_dist*getEntropy(cur_pos,prev_pos)/max_entropy;
        %entropy(i) = entropy(i) + del_dist*getEntropy(cur_pos,prev_pos);
        
        % update our position
        prev_pos = cur_pos;
        cur_pos = cur_pos + v(i) * cyber_period;
    end;
end;

figure(1)
plot(v,entropy)
str = sprintf('Entropy at %G Hz',cyber_rate);
title(str)
xlabel('velocity (m/s)')

%% now vary cyber rate and hold velocity constant
clear variables;
min_cyber_rate = 3;
max_cyber_rate = 30;
rates = min_cyber_rate:0.5:max_cyber_rate;
v = 11;

end_pos = 2000; % end position in meters
start_pos = 0;
total_dist = end_pos - start_pos;
total_time = total_dist/v;

% find the max entropy which would be fastest speed and slowest cyber rate
max_entropy = getMaxEntropy(total_dist,min_cyber_rate,v);

entropy = zeros(1,length(rates));
for i=1:length(rates)
    cur_pos = start_pos;
    prev_pos = start_pos;
    cyber_period = 1/rates(i);
    while cur_pos <= end_pos
        del_dist = (cur_pos - prev_pos)/total_dist;
        entropy(i) = entropy(i) + del_dist*getEntropy(cur_pos,prev_pos)/max_entropy;
        
        % update our position
        prev_pos = cur_pos;
        cur_pos = cur_pos + v * cyber_period;
    end;
end;

figure(2)
plot(rates,entropy)
str = sprintf('Entropy at %G m/s',v);
title(str)
xlabel('x_c (Hz)')
