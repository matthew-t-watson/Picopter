function [phi, psi, bp, bq, P_out] = kalman4( phidot, psidot, accelphi, accelpsi, dt )

phi = zeros(length(dt),1);
psi = zeros(length(dt),1);
bp = zeros(length(dt),1);
bq = zeros(length(dt),1);
P_out = zeros(length(dt),4,4);

Q_phi = 0.0; %Calculated from data
Q_psi = 0.0; %Calculated from data
Q_bp = 0.1;
Q_bq = 0.1;
R_phi = 50; %Accel variance, calculated from data
R_psi = 50; %Accel variance, calculated from data

Q = [Q_phi 0     0    0
     0     Q_psi 0    0
     0     0     Q_bp 0
     0     0     0    Q_bq];

R = [R_phi 0     0 0
     0     R_psi 0 0
     0     0     0 0
     0     0     0 0];

P = [0 0 0 0
     0 0 0 0
     0 0 10000 0
     0 0 0 10000];

H = [1 0 0 0
     0 1 0 0
     0 0 0 0
     0 0 0 0];

I = [1 0 0 0
     0 1 0 0
     0 0 1 0
     0 0 0 1];

x = [accelphi(1)
     accelpsi(1)
     0
     0];

for i = 2:1:length(dt)
    F = [1 0 -dt(i) 0
         0 1 0      -dt(i)
         0 0 1      0
         0 0 0      1];
    
    B = [dt(i) 0     0 0
         0     dt(i) 0 0
         0     0     0 0
         0     0     0 0];
    
    u = [phidot(i)
         psidot(i)
         0
         0];
    
    z = [accelphi(i)
         accelpsi(i)
         0
         0];
    
    %Predicted state estimate
    x = F*x + B*u;
    
    %Predicted estimate covariance
    P = F*P*F' + dt(i)*Q;
    
    %Measurement residual
    y = z - H*x;
    
    %Residual covariance
    S = H*P*H' + R;
    
    %Optimal Kalman gain
    K = P*H'*pinv(S);
    
    %Updated state estimate
    x = x + K*y;
    
    %Updated estimate covariance
    P = (I - K*H)*P;
    
    phi(i) = x(1);
    psi(i) = x(2);
    bp(i) = x(3);
    bq(i) = x(4);
    P_out(i,1:4,1:4) = P(1:4,1:4);
end
end

