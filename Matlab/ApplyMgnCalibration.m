function [ xnew,ynew,znew ] = ApplyMgnCalibration( U, c, x,y,z )

for (i = 1:length(x))
    
    temp0 = x(i) - c(1);
    temp1 = y(i) - c(2);
    temp2 = z(i) - c(3);
    
    xnew(i,1) = U(1,1)*temp0 + U(1,2)*temp1 + U(1,3)*temp2;
    ynew(i,1) = U(2,2)*temp1 + U(2,3)*temp2;
    znew(i,1) = U(3,3)*temp2;

end

