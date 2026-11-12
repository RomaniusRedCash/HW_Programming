using ControlSystems

A_in = 1.0
phi_in_deg = 0.0
f_signals = [0.1, 1.0, 10.0]
fs_signals = [100.0, 100.0, 10000.0]   # fs зависит от сигнала!

systems_z = Dict(
    "H_1" => [[1.0], [1.0]],
    "H_2" => [[0.0, 1.0], [-100.0, 100.0]],
    "H_3" => [[-100.0, 100.0], [0.0, 1.0]],
    "H_5" => [[100.0], [101.0, 1.0]],
    "H_6" => [[1.0], [1.0, 100.0, 10000]],
    "H_7" => [[1.0], [zeros(100); 1.0]],
    )

println("="^60)
println(" РАСЧЕТ ПАРАМЕТРОВ ВЫХОДНОГО СИГНАЛА ДЛЯ РАЗНЫХ ЧАСТОТ (z-область)")
println("="^60 * "\n")
for (f_signal, f_s) in zip(f_signals, fs_signals)
    Ts = 1 / f_s
    omega = 2 * pi * f_signal / f_s

    println("┌──────────────────────────────────────────────────────────")
    println("│ Частота сигнала: f_sig = $f_signal Гц, f_s = $f_s Гц (ω = $(round(omega, digits=4)) рад/отсч)")
    println("└──────────────────────────────────────────────────────────")

    for name in ["H_1", "H_3", "H_6", "H_7"]
        H_val = if name == "H_7"
            exp(-im * 100 * omega)           # z^{-100}, вручную
        else
            sys = tf(systems_z[name]..., Ts)
            freqresp(sys, omega)[1]
        end

        A_out = A_in * abs(H_val)
        phi_out_deg = phi_in_deg + rad2deg(angle(H_val))

        println("  Система $name:")
        println("    * Амплитуда выхода (A_out)  = $(round(A_out, digits=4))")
        println("    * Фаза выхода (φ_out)       = $(round(phi_out_deg, digits=4))°")
    end
    println()
end
