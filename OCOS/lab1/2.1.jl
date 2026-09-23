using Plots

x_1(t) = 0 <= t <= 2 ? 1 : 0
x_2(t) = 0 <= t < 1 ? t : 1 <= t <= 2 ? 2-t : 0
x_3(t) = 0 <= t <= 2 ? sin(2*pi*t) : 0

include("header.jl")

Fs=1000
n=-4000:4000

t_vals = n./Fs

function svertka(x_1, x_2)
    n_1 = length(x_1)
    n_2 = length(x_2)
    A = zeros(Float64, n_1 + n_2 - 1)
    for i in 1:n_1
        for j in 1:n_2
            A[i+j-1]+=x_1[i]*x_2[j]
        end
    end
    return A
end

function korrelyaciya(x1, x2)
    return svertka(x1, reverse(x2))
end

function zprint1(x_1, x_2, name)
    y_1 = svertka(x_1,x_2)
    t = length(y_1)
    t = -(t-1)/2:(t-1)/2
    t=t./Fs
    if saveZad == "y" savefig(plot(t, y_1,title = name,xlabel = "t, с",ylabel = "y(t)",legend = false), "$name.svg")
    else
        display(plot(t, y_1,title = name,xlabel = "t, с",ylabel = "y(t)",legend = false))
        strip(readline()) == "q" && exit(0)
    end
end

function zprint2(x_1, x_2, name)
    y_1 = korrelyaciya(x_1,x_2)
    t = length(y_1)
    t = -(t-1)/2:(t-1)/2
    t=t./Fs
    if saveZad == "y" savefig(plot(t, y_1,title = name,xlabel = "t, с",ylabel = "y(t)",legend = false), "$name.svg")
    else
        display(plot(t, y_1,title = name,xlabel = "t, с",ylabel = "y(t)",legend = false))
        strip(readline()) == "q" && exit(0)
    end
end

function read_signal_csv(filename)
    data = Float64[]
    open(filename, "r") do io
        for line in eachline(io)
            push!(data, parse(Float64, line))
        end
    end
    return data
end

function Zad21()
    x_1_n=x_1.(t_vals)
    x_2_n=x_2.(t_vals)
    x_3_n=x_3.(t_vals)

    zprint1(x_1_n, x_2_n, "x_1*x_2")
    zprint1(x_1_n, x_3_n, "x_1*x_3")
    zprint1(x_2_n, x_3_n, "x_2*x_3")
    zprint1(x_1_n, x_1_n, "x_1*x_1")
    zprint1(x_3_n, x_3_n, "x_3*x_3")

    zprint2(x_1_n,x_2_n,"x_1⋆x_2")
    zprint2(x_3_n,x_3_n,"x_3⋆x_3")

    y_1=read_signal_csv("signal_1.csv")
    y_2=read_signal_csv("signal_2.csv")
    Y = korrelyaciya(y_1,y_2)

    if saveZad == "y" savefig(plot(0:length(y_1)-1,y_1,title="signal_1",legend=false), "signal_1.svg")
    else
        display(plot(0:length(y_1)-1,y_1,title="signal_1",legend=false))
        readline()
    end
    if saveZad == "y" savefig(plot(0:length(y_2)-1,y_2,title="signal_2",legend=false), "signal_2.svg")
    else
        display(plot(0:length(y_2)-1,y_2,title="signal_2",legend=false))
        readline()
    end
    if saveZad == "y" savefig(plot(0:length(Y)-1, Y, title="Корреляция",legend=false), "Корреляция.svg")
    else
        display(plot(0:length(Y)-1, Y, title="Корреляция",legend=false))
        readline()
    end
end
