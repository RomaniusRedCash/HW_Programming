using Plots
include("header.jl")

function Zad12()
    for (idx, f) in enumerate(F)
        f_s, U_min, U_max, M = f
        println("--- Параметры АЦП №$idx ---")
        println("Разрядность (M): $M бит")
        println("Количество уровней (L): $(L(M))")
        println("Шаг квантования (dU): $(dU(U_min, U_max, M)) В\n")
    end

    _t=-1:0.01:4
    x_q(n,f_s,U_min,U_max,M,x_A)=x_D_z(n,f_s,U_min,U_max,M,x_A)*(U_max - U_min)/2^M # 2

    for i in F
        n_range = Int(_t[1]*i[1]):Int(_t[end]*i[1])
        t_vals = n_range ./ i[1]
        x_t(t)=t
        p = plot(t_vals,t_vals, label = "x(t)")
        x_q_t = [x_q(n,i...,x_t) for n in n_range]
        plot!(t_vals, x_q_t, label = "x_q (t)")
        e_q_t = t_vals .- x_q_t
        plot!(t_vals, e_q_t, label = "Ошибка")
        if i[end] < 5 hline!([k * dU(i[2:4]...) for k in 0:(2^i[end])], color=:gray, linestyle=:dot, label="Уровни квантования") end
        if saveZad == "y" savefig(p, "1.2.svg")
        else
            display(p)
            readline()
        end
    end
end
