% script to determine the angular velocity as a function of PWM command
clear all; close all;clc;

%westPWM = 150;

% eastPWM = [300 320 340 360 380 400];
% timePerRev = [1.342 1.271 1.173 1.138 1.121 0.995];
% angularVelocity = 2*pi./timePerRev;

eastPWM = [230 250 300 350 400];
timePerRev = [6.61 5.61 2.677 1.802 1.138];
weights = [0.7 0.8 1 1 0.9];
angularVelocity = 2*pi./timePerRev;


cftool

% PWM(omega) = p1*x + p2
% p1 = 63.69
% p2 = 5.832