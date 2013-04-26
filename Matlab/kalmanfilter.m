function [phi, bias] = kalmanfilter( phidot, accelphi, dt )


Q_angle =  0.000023; %Calculated from data
Q_bias = 0.0001;
R_angle = 24; %Accel variance, calculated from data

Q = [Q_angle 0; 0 Q_bias];


phi=zeros(length(dt),1);
phi(1) = accelphi(1);

bias=zeros(length(dt),1);

P = [0 0
     0 0];
 
C = [1 0]; 


for i = 2:1:length(dt)
    
    A = [1 -dt(i); 0 1];
    
       %Predicted state estimate x = F.x + B.u
       phi(i) = phi(i-1) + dt(i)*(phidot(i) - bias(i-1));
       bias(i) = bias(i-1);
       
       %Predicted estimate covariance P = F.P.F' + Q
       P = A*P*A' + Q;
       
       %Innovation y = z - H.x
       y = accelphi(i) - phi(i);
       
       %Innovation covariance S = H.P.H' + R
       S = C*P*C' + R_angle;
       
       %Optimal kalman gain K = P.H'/S
       K = P * C' / S;
       
       %Updated state estimate x=x + K.y
       phi(i) = phi(i) + K(1)*y;
       bias(i) = bias(i) + K(2)*y;
       
       %Updated estimate covariance P = (I - K.H).P
       P = ([1 0; 0 1] - [K(1); K(2)] * C) * P;
end
end

