using Plots
d(n) = n == 0 ? 1 : 0
x(n) = d(n)
y1(n) = x(n)
y2(x,n) = sum([1/5*x(n-i) for i in 0:5])
y3(n) = -1/4*x(n)
y4(n) = x(n-4)
# y5(n) = n > 0 ? 2*y5(n-1)+x(n) : 0
function F5(N)
    y = zeros(Float64, N)
    for n in 0:N-1
        y[n + 1] = 2.0 * ((n - 1 >= 0) ? y[n] : 0.0) + x(n)
    end
    return y
end
y6(n) = 1/2 * abs(x(n))
b7=[-0.026, -0.6, -0.017, -0.01, -0.022, -0.015, -0.03, -0.023, -0.041, -0.036, -0.059, -0.059, -0.094, -0.117, -0.218, -0.629, 0.629, 0.218, 0.117, 0.094, 0.059, 0.059, 0.036, 0.041, 0.023, 0.03, 0.015, 0.022, 0.01, 0.017, 0.6, 0.026]
y7(x,n) = sum([b7[i+1]*x(n-i) for i in 0:31])
b8 = [0.0646,  -0.1235,    0.1949,   -0.1949,    0.1235,   -0.0646]
a8 = [1.0000 + 0.0000im,   1.4576 - 0.1880im,   2.3598 - 0.2122im,   1.8195 - 0.1694im,   1.0772 - 0.0556im,   0.3218 + 0.0000im]
# y8(n) = n > 0 ? sum([b8[i+1]*x(n-i)+a8[i+1]y[n-i] for i in 0:5]) : 0
b9 = [0.0646,   -0.1235,    0.1949,   -0.1949,    0.1235,   -0.0646]
a9 = [1.0000,    1.4277,    2.1448,    1.5880,    0.9051,    0.2682]
# y9(n) = n > 0 ? sum([b8[i+1]*x(n-i)+a8[i+1]y[n-i] for i in 0:5]) : 0
function F89(b, a, N)
    T = promote_type(Float64, eltype(a))
    y = zeros(T, N)
    for n in 0:N-1
        y[n+1] = sum(b[i+1] * x(n-i) for i in 0:5) - sum(a[i+1] * ((n-i >= 0) ? y[n-i+1] : 0.0) for i in 1:5)
    end
    return real.(y)
end

function zprint(F, N, name, path)
    p = plot(0:N-1, F(N), title = name, xlabel = "Отсчёты", ylabel = "Значение", legend=false)
    if saveZad == "y" savefig(p, "$path.png")
    else
        display(p)
        strip(readline()) == "q" && exit(0)
    end
end

h1(N) = y1.(0:N-1)
h2(x,N) = [y2(x,n) for n in 0:N-1]
h3(N) = y3.(0:N-1)
h4(N) = y4.(0:N-1)
h6(N) = y6.(0:N-1)
h7(x,N) = [y7(x,n) for n in 0:N-1]

function Zad22()
    for (i,F) in enumerate([
            [h1, 15],
            [n -> h2(x,n),15],
            [h3,15],
            [h4,15],
            [F5, 40],
            [h6, 15],
            [n -> h7(x,n), 40],
            [n -> F89(b8,a8,n), 40],
            [n -> F89(b9,a9,n), 40]
            ])
        zprint(F...,"h$i","h$i")
    end
    for (i,x) in enumerate([n -> sin(2*pi*0.01*n), n -> sin(2*pi*0.2*n)])
        for (N,F) in [[2,[n -> h2(x,n),200]],[7,[n -> h7(x,n),200]]]
            zprint(F...,"h$(N)sin","h$(N)sin$i")
        end
    end


end
