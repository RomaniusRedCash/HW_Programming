_t = -1:0.01:4
# x_A(n)=0
T_s(f_s)=1/f_s
M=0

x_D(n,f_s,U_min,U_max,M,x_A)=floor(x_A(n*T_s(f_s))/(U_max - U_min)*2^M) # 1
X_D_z(n,f_s,U_min,U_max,M,x_A)=min(max(x_D(n,f_s,U_min,U_max,M,x_A),0),2^M-1) # 1*

x(n,f_s,U_min,U_max,M,x_A)=x_D(n,f_s,U_min,U_max,M,x_A)*(U_max - U_min)/2^M # 2

L(M)=2^M # 3
dU(U_min,U_max,M)=(U_max - U_min)/2^M # 4

# 1
function d(t)
    return t == 0 ? Inf : 0
end

# 2
function x_const(a, t)
    return t >= 0 ? a : 0
end
A_1=[1,2]

# 3
x_e(a,lambda,t)=a*exp(lambda*t)
A_2=[[2,-0.5],[4,0.5]]

# 4
x_garm(a,f,fi,t)=a*sin(2*pi*f*t+fi)
A_3=[[0.5,1,0],[5,100,0],[1,0.1,pi/4]]

# 5
x_poligarm(t)=cos(2*pi*t)+0.5*cos(2*pi*3*t)+0.2*cos(2*pi*5*t)

# 6
T_max=2
function w_1(t)
    return 0<=abs(t)<=T_max/2 ? 1 : 0
end

function w_2(t)
    return 0<=abs(t)<=T_max/2 ? 0.5*(1+cos(2*pi*t/T_max)) : 0
end

function w_3(t)
    return 0<=abs(t)<=T_max/2 ? 0.54+0.46*cos(2*pi*t/T_max) : 0
end

F=[[1000,-5,5,16],[10,0,3,3]]

using Plots
function zprint(x_A,F)
    display(scatter(_t, [X_D_z(n,F...,x_A) for n in _t],markerstrokewidth = 0, color = :blue, markersize=1, titlefontsize = 9,guidefontsize = 8,tickfontsize = 7,legendfontsize = 7,
        legend=false,
        title = "Выходные коды АЦП",
        xlabel = "Номер отсчета",
        ylabel = "Код АЦП"))
    readline()
end

for i in 1:4
zprint(t -> x_garm(A_3[i]..., t),F[1]) end
zprint(t -> d(t),F[1])
zprint(t -> d(t),F[1])

