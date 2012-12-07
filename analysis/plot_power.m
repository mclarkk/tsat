% plot power
clear variables;close all;clc;
min_vel = 4;
max_vel = 17;
v = min_vel:0.1:max_vel;
figure(1)
plot(v,getPower(v));
title('Power Curve','FontSize',12)
ylabel('Watts','FontSize',12)
xlabel('m/s','FontSize',12)
return;

% now plot energy
% some constants
end_pos = 2000; % end position in meters
start_pos = 0;
total_dist = end_pos - start_pos;
cyber_period = 0.05;
% assign persistent variables
max_energy = getMaxEnergy(total_dist,min_vel,max_vel);

aircraft_energy = zeros(1,length(v));
for i=1:length(v)
    cur_pos = start_pos;
    prev_pos = start_pos;    
    while cur_pos <= end_pos
        aircraft_energy(i) = aircraft_energy(i) + cyber_period/max_energy*getPower(v(i));
        % update our position
        prev_pos = cur_pos;
        cur_pos = cur_pos + v(i) * cyber_period;
    end;
end

figure(2)
plot(v,aircraft_energy)
xlabel('velocity (m/s)')
title('Aircraft Energy')
