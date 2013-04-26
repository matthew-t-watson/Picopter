function [ filtered ] = complementaryfilter(accelangle, gyro, dt, a)

filtered = zeros(length(dt),1);
filtered(1) = accelangle(1);
for i = 2:1:length(dt);
    filtered(i,1) = (filtered(i-1,1) + gyro(i,1)*dt(i,1))*a + (1-a)*accelangle(i,1);
end;

end

