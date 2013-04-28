function [ q, wb, pitch, roll, yaw, error ] = EKFSimple( a,w,dt )


persistent x P;

% Covariance matrices
Q = [0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 2e-9, 0, 0;
    0, 0, 0, 0, 0, 2e-9, 0;
    0, 0, 0, 0, 0, 0, 2e-9];

R = [0.01,    0,    0;
        0, 0.01,    0;
        0,    0, 0.01];
        

if isempty(P)    
    P = eye(7);
    P = P*10000;

    x = [1, 0, 0, 0, 0, 0, 0]';
end

q0 = x(1);
q1 = x(2);
q2 = x(3);
q3 = x(4);
wxb = x(5);
wyb = x(6);
wzb = x(7);
wx = w(1);
wy = w(2);
wz = w(3);

z = [a(1); a(2); a(3)];


%%%%%%%%% PREDICT %%%%%%%%%
% Predicted state estimate
% x = f(x,u)
x = [q0 + (dt/2) * (-q1*(wx-wxb) - q2*(wy-wyb) - q3*(wz-wzb));
     q1 + (dt/2) * ( q0*(wx-wxb) - q3*(wy-wyb) + q2*(wz-wzb));
     q2 + (dt/2) * ( q3*(wx-wxb) + q0*(wy-wyb) - q1*(wz-wzb));
     q3 + (dt/2) * (-q2*(wx-wxb) + q1*(wy-wyb) + q0*(wz-wzb));
     wxb;
     wyb;
     wzb;];

% Normalize quaternion
qnorm = sqrt(x(1)^2 + x(2)^2 + x(3)^2 + x(4)^2);
x(1) = x(1)/qnorm;
x(2) = x(2)/qnorm;
x(3) = x(3)/qnorm;
x(4) = x(4)/qnorm;

q0 = x(1);
q1 = x(2);
q2 = x(3);
q3 = x(4);

% Populate F Jacobian
F = [              1, -(dt/2)*(wx-wxb), -(dt/2)*(wy-wyb), -(dt/2)*(wz-wzb),  (dt/2)*q1,  (dt/2)*q2,  (dt/2)*q3;
     (dt/2)*(wx-wxb),                1,  (dt/2)*(wz-wzb), -(dt/2)*(wy-wyb), -(dt/2)*q0,  (dt/2)*q3, -(dt/2)*q2;
     (dt/2)*(wy-wyb), -(dt/2)*(wz-wzb),                1,  (dt/2)*(wx-wxb), -(dt/2)*q3, -(dt/2)*q0,  (dt/2)*q1;
     (dt/2)*(wz-wzb),  (dt/2)*(wy-wyb), -(dt/2)*(wx-wxb),                1,  (dt/2)*q2, -(dt/2)*q1, -(dt/2)*q0;
                   0,                0,                0,                0,          1,          0,          0;
                   0,                0,                0,                0,          0,          1,          0;
                   0,                0,                0,                0,          0,          0,          1;];


% Predicted covariance estimate
P = F*P*F' + Q;


%%%%%%%%%% UPDATE %%%%%%%%%%%
% Normalize accelerometer and magnetometer measurements
acc_norm = sqrt(z(1)^2 + z(2)^2 + z(3)^2);
z(1) = z(1)/acc_norm;
z(2) = z(2)/acc_norm;
z(3) = z(3)/acc_norm;


h = [2*q0*q2 - 2*q1*q3;
     - 2*q0*q1 - 2*q2*q3;
     - q0^2 + q1^2 + q2^2 - q3^2;];
 
%Measurement residual
% y = z - h(x), where h(x) is the matrix that maps the state onto the measurement
y = z - h;


% Populate H Jacobian
H = [ 2*q2, -2*q3,  2*q0, -2*q1, 0, 0, 0;
     -2*q1, -2*q0, -2*q3, -2*q2, 0, 0, 0;
     -2*q0,  2*q1,  2*q2, -2*q3, 0, 0, 0;];

% Residual covariance
S = H*P*H' + R;

% Calculate Kalman gain
K = P*H'/S;

% Update state estimate
x = x + K*y;

% Normalize quaternion
qnorm = sqrt(x(1)^2 + x(2)^2 + x(3)^2 + x(4)^2);
x(1) = x(1)/qnorm;
x(2) = x(2)/qnorm;
x(3) = x(3)/qnorm;
x(4) = x(4)/qnorm;
q0 = x(1);
q1 = x(2);
q2 = x(3);
q3 = x(4);


% Update estimate covariance
I = eye(length(P));
P = (I - K*H)*P;

% Generate YPR values
pitch = (180/pi) * atan2(2*(q0*q1+q2*q3),1-2*(q1^2+q2^2));
roll = (180/pi) * asin(2*(q0*q2-q3*q1));
yaw = (180/pi) * atan2(2*(q0*q3+q1*q2),1-2*(q2^2+q3^2));


q = [x(1), x(2), x(3), x(4)];
wb = [x(5), x(6), x(7)];
error = y;

end

