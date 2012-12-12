% function getPower(angular_vel) - get power required as a function of
% velocity
% returns power in Watts

function [P] = getPower(angular_vel)

persistent I Vmax PWMmax PWMmin;
if(isempty(I))
    I = 0.2; % Amps
    Vmax = 12; % Max volts
    PWMmax = 400;
    PWMmin = 230;
end;
% order 1 polynomial
p1 = 36.4;%45.28;
p2 = 209.3;%193;
pwm = p1*angular_vel + p2;

% order 2 polynomial
% p1 = -4.138;
% p2 = 63.06;
% p3 = 178.3;
% pwm = p1*angular_vel^2 + p2*angular_vel+ p3; 

% try an exponential (which doesn't fit as well :-( )
% a = 205.2;%223.4;
% b = 0.1553;%0.1107;
% pwm = a*exp(b*angular_vel);

% calculate volts as determined by PWM duty cycle
volts = Vmax*(pwm - PWMmin)/(PWMmax - PWMmin);
P = I*volts;

end