% explore the cost function

clear variables;close all;%clc;

% initialize
global min_vel max_vel;
min_vel = 5;
max_vel = 17;
vels = min_vel:0.1:max_vel;
%vels = min_vel:0.5:max_vel;
global max_cyber_rate min_cyber_rate; % max cyber rate to devote to image acquisition (Hz)
min_cyber_rate = 3;
max_cyber_rate = 20;
cyber_rates = min_cyber_rate:0.1:max_cyber_rate;
%cyber_rates = min_cyber_rate:0.5:max_cyber_rate;

% set weights for Jp
ac_weight_Jp = 10;
time_weight_Jp = 10;

% set weights for Jc
util_weight_Jc = 10;
ent_weight_Jc = 10;

% set weights for the cost function
ac_weight = 10;
time_weight = 10;
util_weight = 10;
ent_weight = 10;

aircraft_energy = zeros(length(cyber_rates),length(vels));
total_time = zeros(length(cyber_rates),length(vels));
cyber_util = zeros(length(cyber_rates),length(vels));
entropy = zeros(length(cyber_rates),length(vels));
Jp = zeros(length(cyber_rates),length(vels));
Jc = zeros(length(cyber_rates),length(vels));
J = zeros(length(cyber_rates),length(vels));
for i = 1:length(cyber_rates)
    for j = 1:length(vels)
        %costs = simulate(vels(i),cyber_rates(j));
        costs = getCosts(vels(j),cyber_rates(i));
        aircraft_energy(i,j) = costs(1,1);
        total_time(i,j) = costs(2,1);
        cyber_util(i,j) = costs(3,1);
        entropy(i,j) = costs(4,1);
        % calculate costs
        Jp(i,j) = ac_weight_Jp*aircraft_energy(i,j)...
                  + time_weight_Jp*total_time(i,j);
        Jc(i,j) = util_weight_Jc*cyber_util(i,j)...
                  + ent_weight_Jc*entropy(i,j);
        J(i,j) = ac_weight*aircraft_energy(i,j)...
                 + time_weight*total_time(i,j)...
                 + util_weight*cyber_util(i,j)...
                 + ent_weight*entropy(i,j);
    end
end

% save('costs','aircraft_energy','total_time','cyber_util',...
%     'entropy','vels','cyber_rates');

figure(1)
surf(vels,cyber_rates,aircraft_energy);
ylabel('Cyber Rate (Hz)')
xlabel('Aircraft Velocity (m/s)')
title('Aircraft Energy')

figure(2)
surf(vels,cyber_rates,total_time);
ylabel('Cyber Rate (Hz)')
xlabel('Aircraft Velocity (m/s)')
title('Total Time')

figure(3)
surf(vels,cyber_rates,cyber_util);
ylabel('Cyber Rate (Hz)')
xlabel('Aircraft Velocity (m/s)')
title('Cyber Utilization')

figure('Position',[200 200 750 600])
surf(vels,cyber_rates,entropy);
view(-125.5,42)
shading interp
ylabel('Cyber Rate (Hz)','FontSize',12)
xlabel('Aircraft Velocity (m/s)','FontSize',12)
zlabel('Entropy','FontSize',12)
title('Entropy','FontSize',12)

figure(5)
surf(vels,cyber_rates,Jp);
ylabel('Cyber Rate (Hz)')
xlabel('Aircraft Velocity (m/s)')
title('J_p')

figure(6)
surf(vels,cyber_rates,Jc);
ylabel('Cyber Rate (Hz)')
xlabel('Aircraft Velocity (m/s)')
title('J_c')

figure('Position',[200 200 750 600])
surf(vels,cyber_rates,J);
view(54.5,30)
shading interp
ylabel('Cyber Rate (Hz)','FontSize',12)
xlabel('Aircraft Velocity (m/s)','FontSize',12)
zlabel('Total Cost','FontSize',12)
title('J','FontSize',12)
