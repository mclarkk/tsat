% plot metrics
clear variables;close all;clc;
clear all;
% these values need to change once we know the min and max angular
% velocities
min_angular_vel = 0.95;
max_angular_vel = 5.52;
angular_vel = min_angular_vel:0.1:max_angular_vel;
power = zeros(1,length(angular_vel));
for i=1:length(angular_vel)
    power(i) = getPower(angular_vel(i));
end
figure(1)
plot(angular_vel,power);
title('Power Curve','FontSize',12)
ylabel('Watts','FontSize',12)
xlabel('Angular Velocity (rads/s)','FontSize',12)

% now plot energy
% some constants
total_dist = 2*pi;
% assign persistent variables
%max_energy = getMaxEnergy(total_dist,min_angular_vel,max_angular_vel);

tsat_energy = zeros(1,length(angular_vel));
for i=1:length(angular_vel)
    time = total_dist/angular_vel(i);
    %tsat_energy(i) = getPower(angular_vel(i))*time/max_energy;
    tsat_energy(i) = getPower(angular_vel(i))*time;
end

figure(2)
plot(angular_vel,tsat_energy)
xlabel('Angular Velocity (rads/s)','FontSize',12)
ylabel('Joules','FontSize',12)
title('Tsat Propulsion Energy','FontSize',12)

figure(3)
subplot(2,1,1)
plot(angular_vel,power);
title('Power and Energy For One Revolution','FontSize',12)
ylabel('Watts','FontSize',12)
xlabel('Angular Velocity (rads/s)','FontSize',12)
subplot(2,1,2)
plot(angular_vel,tsat_energy)
xlabel('Angular Velocity (rads/s)','FontSize',12)
ylabel('Joules','FontSize',12)


