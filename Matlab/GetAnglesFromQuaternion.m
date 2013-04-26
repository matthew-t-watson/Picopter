function [Angles] = GetAnglesFromQuaternion(q)

Angles = zeros(length(q), 3);

for i = 1:length(q)
    q0=q(i,1);
    q1=q(i,2);
    q2=q(i,3);
    q3=q(i,4);
    
    Angles(i,1)=atan2(2*(q2*q3)-2*q0*q1,2*q0^2+2*q3^2-1)*180/pi;
    Angles(i,2)=-asind(2*q1*q3+2*q0*q2);
    Angles(i,3)=atan2(2*q1*q2-2*q0*q3,2*q0^2+2*q1^2-1)*180/pi;
end
end

