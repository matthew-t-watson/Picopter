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
pitch = log(:,17);
roll = log(:,18);
yaw = log(:,19);
pitchdem = log(:,20);
pitchratedem = log(:,21);
rolldem = log(:,22);
rollratedem = log(:,23);
throttle = log(:,24);
yawratedem = log(:,25);
sw1 = log(:,26);
sw2 = log(:,27);
frontleft = log(:,28);
frontright = log(:,29);
rearleft = log(:,30);
rearright = log(:,31);
pidratepitch = log(:,32);
pidrateroll = log(:,33);
pidrateyaw = log(:,34);
pidpitch = log(:,35);
pidroll = log(:,36);
%quat = log(:,37:40);

pint = cumtrapz(p.*dt) + accelphi(1);
qint = cumtrapz(q.*dt) + accelpsi(1);
rint = cumtrapz(r.*dt);

save_to_base(1);

end

