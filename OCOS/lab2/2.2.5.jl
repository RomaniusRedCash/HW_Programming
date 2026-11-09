using ControlSystems

A_in = 1.0
phi_in_deg = 0.0
f_signals = [0.1, 1.0, 10.0]
f_s = 100.0
Ts = 1 / f_s

systems_z = Dict(
    "H_1" => [[1.0], [1.0]],
    "H_2" => [[0.0, 1.0], [-100.0, 100.0]],
    "H_3" => [[-100.0, 100.0], [0.0, 1.0]],
    "H_5" => [[100.0], [101.0, 1.0]],
    "H_6" => [[1.0], [1.0, 100.0, 10000]],
    "H_7" => [[1.0], [zeros(99); 1.0]],
)

println("="^60)
println(" РАСЧЕТ ПАРАМЕТРОВ ВЫХОДНОГО СИГНАЛА ДЛЯ РАЗНЫХ ЧАСТОТ (z-область)")
println("="^60 * "\n")

for f_signal in f_signals
    omega = 2 * pi * f_signal / f_s

    println("┌──────────────────────────────────────────────────────────")
    println("│ Частота сигнала: f_sig = $f_signal Гц (ω = $(round(omega, digits=4)) рад/отсч)")
    println("└──────────────────────────────────────────────────────────")

    for name in ["H_$i" for i in [1, 3, 5, 6, 7]]
        sys = tf(systems_z[name]..., Ts)
        H_val = freqresp(sys, omega)[1]

        A_out = A_in * abs(H_val)
        phi_out_rad = deg2rad(phi_in_deg) + angle(H_val)
        phi_out_deg = rad2deg(phi_out_rad)

        println("  Система $name:")
        println("    * Модуль |H(e^jω)|          = $(round(abs(H_val), digits=4))")
        println("    * Амплитуда выхода (A_out)  = $(round(A_out, digits=4))")
        println("    * Фаза выхода (φ_out)       = $(round(phi_out_deg, digits=2))°")
    end
    println()
end
