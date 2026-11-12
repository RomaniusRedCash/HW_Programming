mkpath("lua/2/1")
mkpath("plt/2/1")

f_signal = 5.0
phi = pi / 4
fs = 200.0
T_duration = 2.0
t = 0:1/fs:T_duration-1/fs

x1 = sin.(2 * pi * f_signal .* t .+ phi)
x2 = cos.(2 * pi * f_signal .* t .+ phi)
x3 = exp.(im .* (2 * pi * f_signal .* t .+ phi))

using Plots
using FFTW

signals = [x1, x2, x3]
names = ["x_$i" for i in range(1,3)]

N = length(t)
frequencies = range(-fs/2, fs/2, length=N)

for (sig, name) in zip(signals, names)
    X = fftshift(fft(sig)) ./ N
    re_part = real.(X)
    im_part = imag.(X)
    amp_spec = abs.(X)
    phase_spec = angle.(X)
    p1 = plot(frequencies, re_part, im_part,
              seriestype = :path3d,
              xlabel="Частота (Гц)", ylabel="Re", zlabel="Im",
              title="3D Спектр: \$$name(t)\$", zlims=(-0.6, 0.6), legend = false)
    p2 = plot(markerstrokewidth=0,frequencies, amp_spec,
              xlabel="Частота (Гц)", ylabel="\$|X(jω)|\$",
              title="Амплитудный спектр", xlims=(-20, 20), markersize = 2, marker = :circle, seriestype = :stem, legend = false)
    p3 = plot(markerstrokewidth=0,frequencies, phase_spec,
              xlabel="Частота (Гц)", ylabel="\$arg(X(jω))\$",
              title="Фазовый спектр", xlims=(-20, 20), markersize = 2, marker = :circle, seriestype = :stem, legend = false)
    savefig(plot(p1, p2, p3, layout=(3, 1), size=(800, 900)), "plt/2/1/$name")
end



systems_s = Dict(
    "H_1" => [[1.0], [1.0]],
    "H_2" => [[1.0], [0.0, 0.5]],
    "H_3" => [[0.0, 0.5], [1.0]],
    "H_4" => [[0.0, 0.1], [1.0, 0.5]],
    "H_5" => [[1.0], [1.0, 0.5]],
)
for (n, a) in [1=>0.0, 2=>0.5, 3=>1.5]
    systems_s["H_{6,a$n}"] = [[1.0], [1.0, a*0.5, 0.25]]
end

function exp_poly_to_string(coeffs)
    res = ""
    for (i, val) in enumerate(coeffs)
        power = i - 1
        if abs(val) < 1e-12
            continue
        end
        sign_str = val < 0 ? "-" : (isempty(res) ? "" : " + ")
        v = abs(val)
        if power == 0
            term = "$v"
        elseif power == 1
            term = "$(v)j\\\\omega"
        else
            term = "$(v)(j\\\\omega)^$(power)"
        end
        res *= sign_str * term
    end
    return isempty(res) ? "0" : res
end

for (name, sys) in systems_s
    num_str = exp_poly_to_string(sys[1])
    den_str = exp_poly_to_string(sys[2])
    open("lua/2/1/sys_$name.lua", "w") do io
        println(io, "return \"\$$name(j\\\\omega) = \\\\frac{$num_str}{$den_str}\$\"")
    end
end

# # using Symbolics
# # @variables w
# # for (name, sys) in systems_s
# #     num = sum(val * (im * w)^(i-1) for (i, val) in enumerate(sys[1]))
# #     den = sum(val * (im * w)^(i-1) for (i, val) in enumerate(sys[2]))
# #
# #     H = num / den
# #
# #     # Раскрываем и упрощаем выражение, чтобы отделить Re от Im
# #     H_expanded = expand(H)
# #     r = real(H_expanded)
# #     i_part = imag(H_expanded)
# #
# #     println("----------------------------------------")
# #     println("Система: $name")
# #     println("H(jω)  = ", simplify(H))
# #
# #     # АЧХ как символьный корень из суммы квадратов Re и Im
# #     A_omega = simplify(sqrt(r^2 + i_part^2))
# #     println("АЧХ(ω) = ", A_omega)
# #
# #     # ФЧХ через atan(Im, Re)
# #     Phi_omega = simplify(atan(i_part, r))
# #     println("ФЧХ(ω) = ", Phi_omega)
# # end




omega = 10 .^ range(-2, 2, length = 1000)
function compute_freq_resp(num_coeffs, den_coeffs, omega)
    A = zeros(length(omega))
    Phi = zeros(length(omega))
    for (idx, w) in enumerate(omega)
        s = im * w
        N = sum(val * s^(i-1) for (i, val) in enumerate(num_coeffs))
        D = sum(val * s^(i-1) for (i, val) in enumerate(den_coeffs))
        H = N / D
        A[idx] = abs(H)
        Phi[idx] = rad2deg(angle(H))
    end
    return A, Phi
end

for name in ["H_$i" for i in range(1,5)]
    sys = systems_s[name]
    A, Phi = compute_freq_resp(sys[1], sys[2], omega)
    L = 20 .* log10.(max.(A, 1e-12))
    p_lachh = plot(title = "ЛАЧХ", xlabel = "\$ω\$", ylabel = "дБ", xscale = :log10, lw = 2, grid = true, ylims = (min(minimum(L), 0.0) - 5.0, max(maximum(L), 0.0) + 5.0))
    p_ph = plot(title = "ФЧХ", xlabel = "\$\\omega\$", ylabel = "\$\\Phi(\\omega)\$", xscale = :log10, lw = 2, grid = true)
    plot!(p_lachh, omega, L, label = "\$$name\$")
    plot!(p_ph, omega, Phi, label = "\$$name\$")
    if name in ["H_4", "H_5"]
        target_L = maximum(L) - 3.01
        w_cut = omega[argmin(abs.(L .- target_L))]
        vline!(p_lachh, [w_cut], label = false, linestyle = :dash, color = :gray)
        scatter!(p_lachh, [w_cut], [target_L], label = "-3 дБ (ω=$(round(w_cut, digits=2)))", color = :black, markersize = 4)
    end
    savefig(plot(p_lachh, p_ph, layout=(2, 1)), "plt/2/1/$name")
end

p_h6_lachh = plot(title = "ЛАЧХ", xlabel = "\$ω\$", ylabel = "дБ", xscale = :log10, lw = 2, grid = true)
p_h6_ph = plot(title = "ФЧХ", xlabel = "\$\\omega\$", ylabel = "\$\\Phi(\\omega)\$", xscale = :log10, lw = 2, grid = true)
for a in [1,2,3]
    sys = systems_s["H_{6,a$a}"]
    A, Phi = compute_freq_resp(sys..., omega)
    L = 20 .* log10.(max.(A, 1e-12))
    plot!(p_h6_lachh, omega, L, label = "\$H_6 (a = $a)\$")
    plot!(p_h6_ph, omega, Phi, label = "\$H_6 (a = $a)\$")
#     if a in [0.5, 1.5]
        target_L = L[1] - 3.01
        w_cut = omega[argmin(abs.(L .- target_L))]
        vline!(p_h6_lachh, [w_cut], label = false, linestyle = :dash, color = :gray)
        scatter!(p_h6_lachh, [w_cut], [target_L], label = false, color = :black, markersize = 4)
#     end
end
savefig(plot(p_h6_lachh, p_h6_ph, layout=(2, 1)), "plt/2/1/H_6")

K = 1.0
tau = 2.0
L_h7 = fill(20 * log10(K), length(omega))
Phi_h7 = -rad2deg.(tau .* omega)
p_lachh = plot(title = "ЛАЧХ (H_7)", xlabel = "ω (рад/с)", ylabel = "дБ", xscale = :log10, lw = 2, grid = true, ylims = (-5, 5))
p_ph = plot(title = "ФЧХ (H_7)", xlabel = "ω (рад/с)", ylabel = "Φ(ω) (град)", xscale = :log10, lw = 2, grid = true)
plot!(p_lachh, omega, L_h7, label = "H_7")
hline!(p_lachh, [0.0], label = false, linestyle = :dot, color = :gray)
plot!(p_ph, omega, Phi_h7, label = "H_7")
savefig(plot(p_lachh, p_ph, layout=(2, 1)), "plt/2/1/H_7.png")

