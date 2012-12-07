% another try and finding pareto fronts

clear variables;close all;clc;

% initialize
global min_vel max_vel;
min_vel = 9;
max_vel = 17;
vels = min_vel:0.1:max_vel;
%vels = min_vel:0.5:max_vel;
global max_cyber_rate min_cyber_rate; % max cyber rate to devote to image acquisition (Hz)
min_cyber_rate = 3;
max_cyber_rate = 20;
cyber_rates = min_cyber_rate:0.1:max_cyber_rate;
%cyber_rates = min_cyber_rate:0.5:max_cyber_rate;

% aircraft_energy = zeros(length(cyber_rates),length(vels));
% total_time = zeros(length(cyber_rates),length(vels));
% cyber_util = zeros(length(cyber_rates),length(vels));
% entropy = zeros(length(cyber_rates),length(vels));
k = 1;
for i = 1:length(cyber_rates)
    for j = 1:length(vels)
        %costs = simulate(vels(i),cyber_rates(j));
        costs = getCosts(vels(j),cyber_rates(i));
        aircraft_energy(k,1) = costs(1,1);
        total_time(k,1) = costs(2,1);
        cyber_util(k,1) = costs(3,1);
        entropy(k,1) = costs(4,1);
        k = k+1;
    end
end

pareto = [aircraft_energy total_time];
front = paretofront(pareto);
figure('Position',[200 200 750 600])
plot(pareto(:,1),pareto(:,2),'c.','LineWidth',2)
hold on
plot(pareto(front,1),pareto(front,2),'ko','LineWidth',4,'MarkerFaceColor','k')
grid on
xlabel('Aircraft Energy Cost','FontSize',12)
ylabel('Time Cost','FontSize',12)
title('Pareto Front of J_p','FontSize',12)

pareto = [cyber_util entropy];
front = paretofront(pareto);
figure('Position',[200 200 750 600])
plot(pareto(:,1),pareto(:,2),'c.','LineWidth',2)
hold on
plot(pareto(front,1),pareto(front,2),'ko','LineWidth',4,'MarkerFaceColor','k')
grid on
xlabel('Cyber Utilization Cost','FontSize',12)
ylabel('Entropy Cost','FontSize',12)
title('Pareto Front of J_c','FontSize',12)

pareto = [aircraft_energy cyber_util entropy];
front = paretofront(pareto);
figure('Position',[200 200 750 600])
plot3(pareto(:,1),pareto(:,2),pareto(:,3),'c.','LineWidth',2)
hold on
plot3(pareto(front,1),pareto(front,2),pareto(front,3),'ko','LineWidth',4,'MarkerFaceColor','k')
grid on
xlabel('Aircraft Energy Cost','FontSize',12)
ylabel('Cyber Utilization Cost','FontSize',12)
zlabel('Entropy Cost','FontSize',12)
title('Pareto Front of J','FontSize',12)

pareto = [aircraft_energy total_time cyber_util];
front = paretofront(pareto);
figure('Position',[200 200 750 600])
plot3(pareto(:,1),pareto(:,2),pareto(:,3),'c.','LineWidth',2)
hold on
plot3(pareto(front,1),pareto(front,2),pareto(front,3),'ko','LineWidth',4,'MarkerFaceColor','k')
grid on
xlabel('Aircraft Energy Cost','FontSize',12)
ylabel('Total Time Cost','FontSize',12)
zlabel('Cyber Utilization Cost','FontSize',12)
title('Pareto Front of J','FontSize',12)    

%pareto = [aircraft_energy total_time entropy];
pareto = [entropy aircraft_energy total_time];
front = paretofront(pareto);
figure('Position',[200 200 750 600])
plot3(pareto(:,1),pareto(:,2),pareto(:,3),'c.','LineWidth',2)
hold on
plot3(pareto(front,1),pareto(front,2),pareto(front,3),'ko','LineWidth',4,'MarkerFaceColor','k')
grid on
ylabel('Aircraft Energy Cost','FontSize',12)
zlabel('Total Time Cost','FontSize',12)
xlabel('Entropy Cost','FontSize',12)
title('Pareto Front of J','FontSize',12)

pareto = [total_time cyber_util entropy];
front = paretofront(pareto);
figure('Position',[200 200 750 600])
%plot3(pareto(:,1),pareto(:,2),pareto(:,3),'b.','MarkerSize',1)
%hold on
plot3(pareto(front,1),pareto(front,2),pareto(front,3),'k.')
view(215,35)
grid on
xlabel('Total Time Cost','FontSize',12)
ylabel('Cyber Utilization Cost','FontSize',12)
zlabel('Entropy Cost','FontSize',12)
title('Pareto Front of J','FontSize',12)