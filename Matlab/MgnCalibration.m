function [U,c] = MgnCalibration(X)
% performs magnetometer calibration from a set of data
% using Merayo technique with a non iterative algoritm
% J.Merayo et al. "Scalar calibration of vector magnemoters"
% Meas. Sci. Technol. 11 (2000) 120-132.
%
%   X      : a Nx3 (or 3xN) data matrix
%              each row (columns) contains x, y, z measurements
%              N must be such that the data set describes
%              as completely as possible the 3D space
%              In any case N > 10
%              
%    The calibration tries to find the best 3D ellipsoid that fits the data set
%    and returns the parameters of this ellipsoid
%
%    U     :  shape ellipsoid parameter, (3x3) upper triangular matrix
%    c      : ellipsoid center, (3x1) vector
%
%    Ellipsoid equation : (v-c)'*(U'*U)(v-c) = 1 
%    with v a rough triaxes magnetometer  measurement
%
%    calibrated measurement w = U*(v-c)
%
%   author : Alain Barraud, Suzanne Lesecq 2008
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[N,m] = size(X);
if m>3&&N==3,X = X';N = m;m = 3;end;%check that X is not transposed
if N<=10,U = [];c = [];return;end;%not enough data no calibration !!
% write  the ellipsoid equation as D*p=0
% the best parameter is the solution of min||D*p|| with ||p||=1;
% form D matrix from X measurements
x = X(:,1); y = X(:,2); z = X(:,3); 
D = [x.^2, y.^2, z.^2, x.*y, x.*z, y.*z, x, y, z, ones(N,1)];
D=triu(qr(D));%avoids to compute the svd of a large matrix
[U,S,V] = svd(D);%because usually N may be very large
p = V(:,end);if p(1)<0,p =-p;end;
% the following matrix A(p) must be positive definite
% The optimization done by svd does not include such a constraint
% With "good" data the constraint is allways satisfied
% With too poor data A may fail to be positive definite
% In this case the calibration fails
%
A = [p(1) p(4)/2 p(5)/2;
       p(4)/2 p(2) p(6)/2; 
       p(5)/2 p(6)/2 p(3)];
[U,ok] = fchol(m,A);
if ~ok,U = [];c = [];return;end%calibration fails too poor data!!
b = [p(7);p(8);p(9)];
v = Utsolve(U,b/2,m);
d = p(10);
s = 1/sqrt(v*v'-d);
c =-Usolve(U,v,m)';%ellipsoid center
U = s*U;%shape ellipsoid parameter
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [A,ok] = fchol(n,A)
% performs Cholesky factoristation
A(1,1:n) = A(1,1:n)/sqrt(A(1,1));
A(2:n,1) = 0;
for j=2:n
  A(j,j:n) = A(j,j:n) - A(1:j-1,j)'*A(1:j-1,j:n);
  if A(j,j)<=0,ok=0;break;end%A is not positive definite
  A(j,j:n) = A(j,j:n)/sqrt(A(j,j));
  A(j+1:n,j) = 0;
end
ok=1;
function x=Utsolve(U,b,n)
% solves U'*x=b
x(1) = b(1)/U(1,1);
for k=2:n
    x(k) = (b(k)-x(1:k-1)*U(1:k-1,k))/U(k,k);
end
function x=Usolve(U,b,n)
% solves U*x=b
x(n) = b(n)/U(n,n);
for k=n-1:-1:1
    x(k) = (b(k)-U(k,k+1:n)*x(k+1:n)')/U(k,k);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%