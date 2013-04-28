function [ quaternion, wb, pitch, roll, yaw, error ] = EKF_CALL( ax,ay,az,p,q,r,mx,my,mz,dt )

a = [ax,ay,az];
w = [p,q,r];
m = [mx,my,mz];
quaternion = zeros(length(dt),4);
wb = zeros(length(dt),3);
pitch = zeros(length(dt),1);
roll = zeros(length(dt),1);
clear EKF;

for i = 1:length(dt)
    [quaternion(i,:), wb(i,:), pitch(i,:), roll(i,:), yaw(i,:), error(i,:)] = EKF(a(i,:),w(i,:),m(i,:),dt(i,1));    
end

end

