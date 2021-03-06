% y'' + (2 + x^2)y + 2 = 0
% y(-1) = y(1) = 0

% Discretization
a = -1;
b = 1;
h = 0.25;
n = (b - a) / h;
x = a : h : b;


%{ 
    y(i+1) - 2*y(i) + y(i-1) / h^2  =  -2 -(2+x(i)^2)y(i)
    y(i+1) - 2*y(i) + y(i-1) = -2h^2 - (2+x(i)^2)y(i)h^2
    
    y(i+1) - (2 - (2 + x(i)^2)h^2)y(i) + y(i-1) = -2h^2
%}


A = zeros(n+1, n+1);
A(1,1) = 1;
for i = 2 : n
    A(i,[i-1 i i+1]) = [1 -(2 -(2+x(i)^2)*(h^2))  1];
end
A(n+1,n+1) = 1;

% right hand side
B = zeros(n+1, 1);
B(1) = 0;
B(2:n) = -2*h^2 ;
B(n+1) = 0;

A
B

% Now I have the matrix AX = B


