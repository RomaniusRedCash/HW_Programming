using Plots
using DSP
using SpecialFunctions
using FFTW

fs = 200.0
T1 = 2.0
t1 = 0:1/fs:(T1 - 1/fs)
function plot_spectrum(signal, fs, title_str; window_sum = nothing)
    N = length(signal)
    X = fft(signal)
    frequencies = range(0, fs/2, length = div(N, 2) + 1)

    norm_factor = window_sum === nothing ? N : window_sum
    amp_spectrum = abs.(X[1:div(N, 2) + 1]) .* (2 / norm_factor)
    amp_spectrum[1] /= 2

    plot(frequencies, seriestype = :stem, amp_spectrum,
         xlabel = "Частота (Гц)", ylabel = "Амплитуда", title = title_str,
         xlims = (0, 40), marker = :circle, markersize = 3, legend = false)
end

x1 = sin.(2 * pi * 1.0 * t1) .+ 0.5 * sin.(2 * pi * 10.0 * t1) .+ 0.3 * sin.(2 * pi * 30.0 * t1)
p1 = plot_spectrum(x1, fs, "Базовый сигнал (T=2с, f = [1, 10, 30] Гц)")
x2 = sin.(2 * pi * 1.25 * t1) .+ 0.5 * sin.(2 * pi * 10.0 * t1) .+ 0.3 * sin.(2 * pi * 30.0 * t1)
p2 = plot_spectrum(x2, fs, "Изменение частоты на 1.25 Гц")
x2_zp = vcat(x2, zeros(4096 - length(x2)))
p3 = plot_spectrum(x2_zp, fs, "Zero-padding до N=4096")
T_long = 8.0
t_long = 0:1/fs:(T_long - 1/fs)
x_long = sin.(2 * pi * 1.25 * t_long) .+ 0.5 * sin.(2 * pi * 10.0 * t_long) .+ 0.3 * sin.(2 * pi * 30.0 * t_long)
x_long_zp = vcat(x_long, zeros(16384 - length(x_long)))
p4 = plot_spectrum(x_long_zp, fs, "Увеличение T=8с и N=16384")

N_win = 4096
n_idx = 0:(length(x2)-1)
w_hann = 0.5 * (1 .- cos.(2 * pi * n_idx / (length(x2) - 1)))
w_blackman = 0.42 .- 0.5 * cos.(2 * pi * n_idx / (length(x2) - 1)) .+ 0.08 * cos.(4 * pi * n_idx / (length(x2) - 1))
beta = 5.0
I0_beta = besseli(0, beta)
w_kaiser = [besseli(0, beta * sqrt(max(0, 1 - ((2*n / (length(x2) - 1)) - 1)^2))) / I0_beta for n in n_idx]

for (name, win, fname) in [("Окно Ханна", w_hann, "hann"), ("Окно Блэкмана", w_blackman, "black"), ("Окно Кайзера", w_kaiser, "kaiser")]
    w = x2 .* win
    w = vcat(w, zeros(N_win - length(w)))
    savefig(plot_spectrum(w, fs, name, window_sum = sum(win)), "plt/2/3/$fname")
end
