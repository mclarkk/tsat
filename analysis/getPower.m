% function getPower(volts) - get power required as a function of
% voltage
% returns power in Watts

function [P] = getPower(angular_vel)

persistent I;
if(isempty(e))
    I = 0.2; % Amps
end;
P = I*volts;

end