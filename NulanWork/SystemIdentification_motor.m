clear all;close all;clc;

data = readtable("data.csv");
steps = data.Var1;
time = 0:size(data) - 1;
time = time';

plot(time, steps)

% The equation: y = 0.01146x -35.75, --> 0.01146 corresponds to the
% gradient, which should be equal to the max speed = 4011.625counts/sec,
% 0.01146counts/div * 1/4011.625 sec/counts = 2.8567e-6 sec/div

updated_time = 0:2.8567e-6:2.8567e-6 * (size(data) - 1);
updated_time = updated_time';

plot(updated_time, steps)
%%
transfer = tf(tf1.Numerator, tf1.Denominator);
[A, B, C, D] = tf2ss(tf1.Numerator, tf1.Denominator);

a0 = for_accel.p5;
a1 = for_accel.p4;
a2 = for_accel.p3;
a3 = for_accel.p2;
a4 = for_accel.p1;

syms f x

f =  a0 + a1 * x + a2 * x.^2 + a3 * x.^3 + a4*x.^4;
vel = diff(f);
accel = diff(vel);
subplot(1, 3, 1)
fplot(f, [0, updated_time(end)])
subplot(1, 3, 2)
fplot(vel, [0, updated_time(end)])
subplot(1, 3, 3)
fplot(accel, [0, updated_time(end)])