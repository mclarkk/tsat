% explore the cost function

clear variables;close all;%clc;
clear all;

% initialize
global min_angular_vel max_angular_vel;
min_angular_vel = 0.95;
max_angular_vel = 5.52;
angular_vels = min_angular_vel:0.1:max_angular_vel;
global max_cyber_rate min_cyber_rate; % max cyber rate to devote to image acquisition (Hz)
min_cyber_rate = 1;
max_cyber_rate = 8;
cyber_rates = min_cyber_rate:0.1:max_cyber_rate;

% set weights for Jp
tsat_weight_Jp = 1;
time_weight_Jp = 1;

% set weights for Jc
util_weight_Jc = 1;
info_weight_Jc = 1;

% set weights for the cost function
tsat_weight = 1;
time_weight = 1.2;
util_weight = 0.8;
info_weight = 1;

tsat_energy = zeros(length(cyber_rates),length(angular_vels));
total_time = zeros(length(cyber_rates),length(angular_vels));
cyber_util = zeros(length(cyber_rates),length(angular_vels));
information = zeros(length(cyber_rates),length(angular_vels));
Jp = zeros(length(cyber_rates),length(angular_vels));
Jc = zeros(length(cyber_rates),length(angular_vels));
J = zeros(length(cyber_rates),length(angular_vels));
for i = 1:length(cyber_rates)
    for j = 1:length(angular_vels)
        costs = getCosts(angular_vels(j),cyber_rates(i));
        tsat_energy(i,j) = costs(1,1);
        total_time(i,j) = costs(2,1);
        cyber_util(i,j) = costs(3,1);
        information(i,j) = costs(4,1);
        % calculate costs
        Jp(i,j) = tsat_weight_Jp*tsat_energy(i,j)...
                  + time_weight_Jp*total_time(i,j);
        Jc(i,j) = util_weight_Jc*cyber_util(i,j)...
                  + info_weight_Jc*information(i,j);
        J(i,j) = tsat_weight*tsat_energy(i,j)...
                 + time_weight*total_time(i,j)...
                 + util_weight*cyber_util(i,j)...
                 + info_weight*information(i,j);
    end
end

% save('costs','aircraft_energy','total_time','cyber_util',...
%     'entropy','vels','cyber_rates');

figure(1)
surf(angular_vels,cyber_rates,tsat_energy);
ylabel('Cyber Rate (Hz)')
xlabel('Angular Velocity (rads/s)')
title('TableSat Energy (J)')

figure(2)
surf(angular_vels,cyber_rates,total_time);
ylabel('Cyber Rate (Hz)')
xlabel('Angular Velocity (rads/s)')
title('Total Time')

figure(3)
surf(angular_vels,cyber_rates,cyber_util);
ylabel('Cyber Rate (Hz)')
xlabel('Angular Velocity (rads/s)')
title('Cyber Utilization')

%figure('Position',[200 200 750 600])
figure(4)
surf(angular_vels,cyber_rates,information);
%view(-125.5,42)
%shading interp
ylabel('Cyber Rate (Hz)','FontSize',12)
xlabel('Angular Velocity (rads/s)','FontSize',12)
title('Information^{-1}','FontSize',12)

figure(5)
surf(angular_vels,cyber_rates,Jp);
ylabel('Cyber Rate (Hz)')
xlabel('Angular Velocity (rads/s)')
title('J_p')

figure(6)
surf(angular_vels,cyber_rates,Jc);
ylabel('Cyber Rate (Hz)')
xlabel('Angular Velocity (rads/s)')
title('J_c')

%figure('Position',[200 200 750 600])
figure(7)
surf(angular_vels,cyber_rates,J);
%view(54.5,30)
%shading interp
ylabel('Cyber Rate (Hz)','FontSize',12)
xlabel('Angular Velocity (rads/s)','FontSize',12)
zlabel('Total Cost','FontSize',12)
title('J','FontSize',12)
