function d = f(t,x,delta_t,v,theta,l)
if(t > 0.5)
	theta = ;    
end
d = [v*cos(x(3) - theta); v*sin(x(3) - theta); -v*sin(theta)/l];
end