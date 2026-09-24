include("header.jl")

# 1
d(t) = t == 0 ? Inf : 0
d_digit(t) = t == 0 ? 1 : 0

# 2
x_const(a, t) = t >= 0 ? a : 0
A_1=[1,2]

# 3
x_e(a,lambda,t) = a * exp(lambda*t)
A_2=[[2,-0.5],[4,0.5]]

# 4
x_garm(a,f,fi,t)= a * sin(2*pi*f*t+fi)
A_3=[[0.5,1,0],[5,100,0],[1,0.1,pi/4]]

# 5
x_poligarm(t) = cos(2*pi*t) + 0.5 * cos(2*pi*3*t) + 0.2 * cos(2*pi*5*t)

# 6
T_max=2
w_1(t) = 0 <= abs(t) <= T_max/2 ? 1 : 0

w_2(t) = 0 <= abs(t) <= T_max/2 ? 0.5*(1+cos(2*pi*t/T_max)) : 0

w_3(t) = 0 <= abs(t) <= T_max/2 ? 0.54+0.46 * cos(2*pi*t/T_max) : 0

using Plots
Setting=(markerstrokewidth=0,color = :blue,markersize=2,titlefontsize=9,guidefontsize=8,tickfontsize=7,legendfontsize=7,legend=false)
function zprint1(x_A,F,_t,name)
    n_range = Int(_t[1]*F[1]):Int(_t[end]*F[1])
    return scatter(n_range, [x_D(n,F...,x_A) for n in n_range];
                       Setting...,
                       title = name, xlabel = "Шаг", ylabel = "Код")
end
function zprint2(x_A,F,_t,name)
    n_range = Int(_t[1]*F[1]):Int(_t[end]*F[1])
    t_vals = n_range ./ F[1]
    return scatter(t_vals, [x(n,F...,x_A) for n in n_range];
        Setting...,
        title = name,xlabel = "t,с",ylabel = "U,В")
end

function zprint(x_A,F,_t,name)
#     display(plot(zprint1(x_A,F,_t,name),zprint2(x_A,F,_t,name),layout = (1,2)))
    if saveZad == "y" savefig(zprint2(x_A,F,_t,name), "$name.svg")
    else
        display(zprint2(x_A,F,_t,name))
        strip(readline()) == "q" && exit(0)
    end
end

function Zad11()
    for (n,i) in enumerate(F)
        zprint(t -> d_digit(t), i, -2:2, "Дельта, F_$n") # 1
        for j in A_1 zprint(t -> x_const(j, t), i, -2:2, "Константа, $j, F_$n") end # 2
        for j in A_2 zprint(t -> x_e(j..., t), i, -1:10, "Экспонента, $(join(j, ", ")), F_$n") end # 3
        for j in A_3 zprint(t -> x_garm(j..., t), i, -5:5, "Гармонические колебания, $(join(j, ", ")), F_$n") end # 4
        zprint(t -> x_poligarm(t), i, -5:5, "Полигармонические, F_$n") # 5
        for (w,j) in [[w_1, "прямоугольное"], [w_2, "Ханна"], [w_3, "Хэмминга"]] zprint(t -> w(t), i, -5:2, "Окно $j, F_$n") end # 6
    end
    # zprint(t -> x_garm(A_3[2]..., t), F[1], -0.01:0.001:0.01, "Гармонические колебания, $(join(A_3[2], ", "))") # fix очень высокая частота у синуса
end
