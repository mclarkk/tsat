% plot entropy
clear variables; close all;clc;
% some constants
min_angular_vel = 1;
max_angular_vel = 8;
angular_vel = min_angular_vel:0.1:max_angular_vel;
cyber_rate = 3;

total_dist = 4*pi;
cyber_period = 1/cyber_rate;

% find the max info which would be max cyber rate, min angular velocity
max_info = getMaxInformation(total_dist,cyber_rate,max_angular_vel);

information = zeros(1,length(angular_vel));
for i=1:length(angular_vel)
    information(i) = getInformation(angular_vel(i),cyber_rate)*total_dist/max_info;

end;

figure(1)
plot(angular_vel,information)
str = sprintf('Information at %G Hz',cyber_rate);
title(str)
xlabel('velocity (rads/s)')
information1 = information;
angular_vel_vec = angular_vel;

%% now vary cyber rate and hold velocity constant
%clear variables;
min_cyber_rate = 0.5;
max_cyber_rate = 10;
rates = min_cyber_rate:0.1:max_cyber_rate;
angular_vel = 5;

total_dist = 4*pi;
total_time = total_dist/angular_vel;

% find the max info which would be max cyber rate, min angular velocity
max_info = getMaxInformation(total_dist,min_cyber_rate,angular_vel);

information = zeros(1,length(rates));
for i=1:length(rates)
    information(i) = getInformation(angular_vel,rates(i))*total_dist/max_info;
end;

figure(2)
plot(rates,information)
str = sprintf('Information at %G rads/s',angular_vel);
title(str)
xlabel('r_{\tau} (Hz)')

figure('Position',[150 250 500 650])
h1=subplot(2,1,1)
plot(angular_vel_vec,information1)
str = sprintf('Information at %G Hz',cyber_rate);
th1=title(str,'FontSize',12)
movetitleinside(h1,th1);
xlabel('velocity (rads/s)','FontSize',12)
h2=subplot(2,1,2)
plot(rates,information)
str = sprintf('Information at %G rads/s',angular_vel);
th2=title(str,'FontSize',12)
movetitleinside(h2,th2);
xlabel('r_{\tau} (Hz)','FontSize',12)