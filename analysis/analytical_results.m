clear all;close all;clc;
min_angular_vel = 0.95;
max_angular_vel = 5.52;
min_cyber_rate = 1;
max_cyber_rate = 8;
dist = 2*pi;

max_time = dist/min_angular_vel;
max_energy = getMaxEnergy(dist,min_angular_vel,max_angular_vel);
max_info = getMaxInformation(dist,min_cyber_rate,max_angular_vel);

energy_weight = 0.25;
time_weight = 0.25;
util_weight = 0.25;
info_weight = 0.25;

% case 1
omega = 5.2
rate = 2
time = dist/omega
energy = time*getPower(omega)
util = rate/max_cyber_rate
info = getInformation(omega,rate)
total1 = energy_weight*energy/max_energy...
        + util_weight*util...
        + info_weight*info/max_info...
        + time_weight*time/max_time

% case 2
omega = 0.95
rate = 3
time = dist/omega
energy = time*getPower(omega)
util = rate/max_cyber_rate
info = getInformation(omega,rate)
total2 = energy_weight*energy/max_energy...
        + util_weight*util...
        + info_weight*info/max_info...
        + time_weight*time/max_time

% case 3
omega = 2.418
rate = 2
time = dist/omega
energy = time*getPower(omega)
util = rate/max_cyber_rate
info = getInformation(omega,rate)
total3 = energy_weight*energy/max_energy...
        + util_weight*util...
        + info_weight*info/max_info...
        + time_weight*time/max_time

totals = [total1 total2 total3];
best = min(totals);
percents = (totals-best)/best
    