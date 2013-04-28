function convlog(log)

sample = log(:,1);
dt = log(:,2)/1000;
x = log(:,3);
y = log(:,4);
z = log(:,5);
p = log(:,6);
q = log(:,7);
r = log(:,8);
temp = log(:,9);
magx = log(:,10);
magy = log(:,11);
magz = log(:,12);
pressure = log(:,13);
altitude = log(:,14);
pitch = log(:,15);
roll = log(:,16);
yaw = log(:,17);
pitchdem = log(:,18);
pitchratedem = log(:,19);
rolldem = log(:,20);
rollratedem = log(:,21);
throttle = log(:,22);
yawratedem = log(:,23);
sw1 = log(:,24);
sw2 = log(:,25);
frontleft = log(:,26);
frontright = log(:,27);
rearleft = log(:,28);
rearright = log(:,29);
pidratepitch = log(:,30);
pidrateroll = log(:,31);
pidrateyaw = log(:,32);
pidpitch = log(:,33);
pidroll = log(:,34);
quat = log(:,35:38);

accelpitch = (180/pi) * atan2(y, sqrt(x.^2 + z.^2));
accelroll = (180/pi) * atan2(x, sqrt(y.^2 + z.^2));

pint = cumtrapz(p.*dt) + accelpitch(1);
qint = cumtrapz(q.*dt) + accelroll(1);
rint = cumtrapz(r.*dt);



save_to_base(1);

end

