clear all;close all;clc;
min_angular_vel = 0.95;
max_angular_vel = 5.52;
min_cyber_rate = 1;
max_cyber_rate = 8;
dist = 2*pi;

max_time = dist/min_angular_vel;
max_energy = getMaxEnergy(dist,min_angular_vel,max_angular_vel);
max_info = getMaxInformation(dist,min_cyber_rate,max_angular_vel);

% case 1
omega = 5.52
rate = 2
time = dist/omega
energy = time*getPower(omega)
util = rate/max_cyber_rate
info = 1/363224
total = energy + util+info+time

% case 2
omega = 0.95
rate = 3
time = dist/omega
energy = time*getPower(omega)
util = rate/max_cyber_rate
info = 1/4239584
total = energy + util+info+time

% case 3
omega = 2.418
rate = 2
time = dist/omega
energy = time*getPower(omega)
util = rate/max_cyber_rate
info = 1/585893
total = energy + util+info+time
