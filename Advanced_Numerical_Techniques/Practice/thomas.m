% Discretization
a = 0;
b = 1;
n = 10;
dx = (b - a) / n;
x = a : dx : b;

% Building up the matrix form
% y'' = 12x^2 --> y(0) = 0, y(1) = 0
%{
    [0,1] - interval
    dx = 1/J
    x(j) = 0 + (j-1)dx  j = 1.....J+1
    lets approximate Y(j) = y(x(j))
    for y'' = 12x^2
    by 
    Y(j-1) - 2Y(j) + Y(j+1) / dx^2 = 12x(j)^2
    BC Y(1) = Y(J+1) = 0

    Y1 = 0
    Y1 - 2Y2 + Y3 = 12X2^2 dx^2
    .
    .
    YJ+1 = 0

    [1 0 0 ....0    [Y1]        [.]
     1 -2 1 0..0    [Y2]        [.]
    0 1 -2 1 ..0                [.]
    .                       = 
    .
    0 .....  0 1]   [YJ+1]      [.]
    AY = B
%}
A = sparse(n+1, n+1);
A(1,1) = 1;
for i = 2 : n
    A(i,[i-1 i i+1]) = [1 -2 1];
end
A(n+1,n+1) = 1;

% right hand side
B = zeros(n+1, 1);
B(1) = 0;
B(2:n) = 12.*x(2:n).^2.*dx^2;
B(n+1) = 0;

% Solving the System
U = A\B;

% Comapring with the exact solution
xf = a : 1/1000 : b;
yf = xf.^4 - xf;
plot(x,U,'O',xf,yf)
xlabel('x'); grid on; legend('Finite diff', 'Exact')


