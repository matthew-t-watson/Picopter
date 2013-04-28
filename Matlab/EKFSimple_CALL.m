function [ quaternion, wb, pitch, roll, yaw, error ] = EKFSimple_CALL( ax,ay,az,p,q,r,dt )

a = [ax,ay,az];
w = [p,q,r];
quaternion = zeros(length(dt),4);
wb = zeros(length(dt),3);
pitch = zeros(length(dt),1);
roll = zeros(length(dt),1);
clear EKFSimple;

for i = 1:length(dt)
    [quaternion(i,:), wb(i,:), pitch(i,:), roll(i,:), yaw(i,:), error(i,:)] = EKFSimple(a(i,:),w(i,:),dt(i,1));    
end

end

