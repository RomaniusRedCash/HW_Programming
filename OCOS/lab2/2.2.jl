mkpath("lua/2/2")
mkpath("plt/2/2")

systems_z = Dict(
    "H_1" => [[1.0], [1.0]],
    "H_2" => [[0.0, 1.0], [-100.0, 100.0]],
    "H_3" => [[-100.0, 100.0], [0.0, 1.0]],
    "H_5" => [[100.0], [101.0, 1.0]],
    "H_6" => [[1.0], [1.0, 100.0, 10000]],
    "H_7" => [[1.0], [zeros(100); 1.0]],
)


function exp_poly_to_string(coeffs)
    res = ""
    for (i, val) in enumerate(coeffs)
        power = i - 1
        if abs(val) < 1e-12
            continue
        end
        sign_str = val < 0 ? "-" : (isempty(res) ? "" : " + ")
        v = abs(val)
        v = (v == 1.0 && power > 0) ? "" : "$v"
        if power == 0
            term = "$v"
        elseif power == 1
            term = "$(v)e^{j\\\\omega}"
        else
            term = "$(v)e^{j$(power)\\\\omega}"
        end
        res *= sign_str * term
    end
    return isempty(res) ? "0" : res
end

for (name, sys) in systems_z
    num_str = exp_poly_to_string(sys[1])
    den_str = exp_poly_to_string(sys[2])
    open("lua/2/2/sys_$name.lua", "w") do io
        println(io, "return \"\$$name(e^{j\\\\omega}) = \\\\frac{$num_str}{$den_str}\$\"")
    end
end

# # using Symbolics
# # println("--- АНАЛИТИЧЕСКИЙ РАСЧЕТ ДИСКРЕТНЫХ СИСТЕМ ---\n")
# # @variables w
# # for (name, sys) in systems_z
# #     b_coeffs = sys[1]
# #     a_coeffs = sys[2]
# #
# #     num = sum(b_coeffs[i] * exp(-im * (i - 1) * w) for i in 1:length(b_coeffs))
# #     den = sum(a_coeffs[i] * exp(-im * (i - 1) * w) for i in 1:length(a_coeffs))
# #
# #     H_z = num / den
# #
# #     A_omega = abs(H_z)
# #     Phi_omega = atan(imag(H_z), real(H_z))
# #
# #     println("----------------------------------------")
# #     println("Система: $name")
# #     println("H(e^jw) = ", simplify(H_z))
# #     println("A(ω)    = ", simplify(A_omega))
# #     println("Φ(ω)    = ", simplify(Phi_omega))
# # end

using Plots

omega = range(-pi, pi, length = 1000)

function compute_freq_resp_z(num_coeffs, den_coeffs, omega)
    A = zeros(length(omega))
    Phi = zeros(length(omega))
    for (idx, w) in enumerate(omega)
        z = exp(im * w)
        N = sum(val * z^-(i-1) for (i, val) in enumerate(num_coeffs))
        D = sum(val * z^-(i-1) for (i, val) in enumerate(den_coeffs))
        H = N / D
        A[idx] = abs(H)
        Phi[idx] = rad2deg(angle(H))
    end
    return A, Phi
end

for name in ["H_$i" for i in 1:7]
    if name == "H_4"
        continue
    end
    sys = systems_z[name]
    A, Phi = compute_freq_resp_z(sys[1], sys[2], omega)
    L = 20 .* log10.(max.(A, 1e-12))
    p_lachh = plot(title = "ЛАЧХ ($name)", xlabel = "\$ω\$ (рад/отсч)", ylabel = "дБ", lw = 2, grid = true, ylims = (min(minimum(L), 0.0) - 5.0, max(maximum(L), 0.0) + 5.0))
    p_ph = plot(title = "ФЧХ ($name)", xlabel = "\$ω\$ (рад/отсч)", ylabel = "\$\\Phi(\\omega)\$ (град)", lw = 2, grid = true)
    plot!(p_lachh, omega, L, label = name)
    plot!(p_ph, omega, Phi, label = name)
    if name in ["H_3"]
        pos_mask = omega .>= 0
        w_pos = omega[pos_mask]
        L_pos = L[pos_mask]
        target_L = maximum(L_pos) - 3.01
        w_cut = w_pos[argmin(abs.(L_pos .- target_L))]
        vline!(p_lachh, [w_cut, -w_cut], label = false, linestyle = :dash, color = :gray)
        scatter!(p_lachh, [w_cut, -w_cut], [target_L], label = false, color = :black, markersize = 4)
    end
    savefig(plot(p_lachh, p_ph, layout=(2, 1)), "plt/2/2/$name.png")
end
for fs in (100.0, 10_000.0)
    f = omega ./ (2π) .* fs            # Гц
    tag = fs == 100.0 ? "fs100" : "fs10k"

    for name in ["H_$i" for i in 1:7]
        name == "H_4" && continue
        sys = systems_z[name]
        A, Phi = compute_freq_resp_z(sys[1], sys[2], omega)
        L = 20 .* log10.(max.(A, 1e-12))

        p_lachh = plot(title = "ЛАЧХ ($name), \$f_s = $fs\$ Гц",
                       xlabel = "\$f\$ (Гц)", ylabel = "дБ", lw = 2, grid = true,
                       ylims = (min(minimum(L), 0.0) - 5.0, max(maximum(L), 0.0) + 5.0))
        p_ph = plot(title = "ФЧХ ($name), \$f_s = $fs\$ Гц",
                    xlabel = "\$f\$ (Гц)", ylabel = "\$\\Phi(\\omega)\$ (град)", lw = 2, grid = true)

        plot!(p_lachh, f, L,   label = name)
        plot!(p_ph,    f, Phi, label = name)

        if name == "H_3"
            pos_mask = f .>= 0
            f_pos = f[pos_mask];  L_pos = L[pos_mask]
            target_L = maximum(L_pos) - 3.01
            f_cut = f_pos[argmin(abs.(L_pos .- target_L))]
            vline!(p_lachh, [f_cut, -f_cut], label = false, linestyle = :dash, color = :gray)
            scatter!(p_lachh, [f_cut, -f_cut], [target_L], label = false, color = :black, markersize = 4)
        end

        savefig(plot(p_lachh, p_ph, layout = (2, 1)),
                "plt/2/2/$(name)_$tag.png")
    end
end
