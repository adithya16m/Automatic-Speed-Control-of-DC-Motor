%% Automatic DC Motor Speed Control - Control Concept
% This script provides a lightweight MATLAB representation of the
% incremental feedback algorithm described in the supplied project report.
%
% It is NOT the original Simulink model. The supplied files describe and
% show the Simulink model, but do not contain the original .slx file.

clear; clc; close all;

Ts = 0.01;                 % simulation step (s)
Tend = 5;                  % simulation time (s)
t = 0:Ts:Tend;

targetVoltage = 5;        % target motor voltage (V)

% Example variable input voltage, representing supply fluctuations.
Vin = 7 + 1.5*sin(2*pi*0.4*t);

motorVoltage = zeros(size(t));
pwm = zeros(size(t));

dutyStep = 0.02;
deadband = 0.05;

% Simple illustrative motor/plant response.
% This is only a control-concept model, not an identified motor model.
plantGain = 1.0;
timeConstant = 0.15;

for k = 2:length(t)

    error = targetVoltage - motorVoltage(k-1);

    if error > deadband
        pwm(k) = pwm(k-1) + dutyStep;
    elseif error < -deadband
        pwm(k) = pwm(k-1) - dutyStep;
    else
        pwm(k) = pwm(k-1);
    end

    pwm(k) = min(max(pwm(k), 0), 1);

    commandedVoltage = plantGain * pwm(k) * Vin(k);

    % First-order plant approximation
    motorVoltage(k) = motorVoltage(k-1) + ...
        (Ts/timeConstant) * (commandedVoltage - motorVoltage(k-1));
end

figure;
plot(t, Vin, 'DisplayName', 'Input Voltage');
hold on;
plot(t, motorVoltage, 'DisplayName', 'Motor Voltage');
yline(targetVoltage, '--', 'DisplayName', 'Target Voltage');
grid on;
xlabel('Time (s)');
ylabel('Voltage (V)');
title('Feedback-Based DC Motor Voltage Control');
legend('Location', 'best');

figure;
plot(t, 100*pwm);
grid on;
xlabel('Time (s)');
ylabel('PWM Duty Cycle (%)');
title('PWM Duty Cycle Response');
