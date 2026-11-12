using Polynomials

mkpath("lua/1/3")

den = [1.0, 3.2361, 5.2361, 5.2361, 3.2361, 1.0]

open("lua/1/3/lap.lua", "w") do io
    print(io, "return \"\$")
    for (idx, val) in enumerate(den)
        power = length(den) - idx
        if power > 1
            term = "$(val)s^$(power)Y(s)+"
        elseif power == 1
            term = "$(val)sY(s)+"
        else
            term = "$(val)Y(s)=X(s)"
        end
        print(io, term)
    end
    print(io, "\$\"")
end

_poles = roots(Polynomial(den))

function format_pole(p)
    re = round(real(p), digits=4)
    im = round(imag(p), digits=4)
    if im == 0
        return "\"" * string(re) * "\""
        elseif re == 0
        return "\"" * string(im) * "i" * "\""
    else
        sign_str = im > 0 ? "+" : "-"
        return "\"" * string(re) * sign_str * string(abs(im)) * "i" * "\""
    end
end

real_poles = filter(p -> abs(imag(p)) < 1e-6, _poles)
formatted_poles = join([format_pole(p) for p in _poles], ", ")
open("lua/1/3/poles.lua", "w") do io
    println(io, "return {poles = {$(formatted_poles)}, is_stable = $(all(real.(_poles) .< 0))}")
end

eal_poles = filter(p -> abs(imag(p)) < 1e-6, _poles)
complex_poles = filter(p -> abs(imag(p)) >= 1e-6, _poles)
println("Звенья 2-го порядка:")
seen = Set()
for p in complex_poles
    if p in seen || conj(p) in seen; continue; end
    push!(seen, p)
    a = -2 * real(p)
    b = abs2(p)
    println("  H_i(s) = 1 / (s^2 + $(round(a, digits=4))s + $(round(b, digits=4)))")
end
println("\nЗвенья 1-го порядка:")
for p in real_poles
    a = -real(p)
    println("  H_j(s) = 1 / (s + $(round(a, digits=4)))")
end



using ControlSystemsBase
num_s = [1.0]
Hs = tf(num_s, den)
Ts = 0.1
Hz = c2d(Hs, Ts, :tustin)
num_p = numpoly(Hz)[1]
den_p = denpoly(Hz)[1]
b_coeffs = coeffs(num_p)
a_coeffs = coeffs(den_p)

function poly_to_string(coeffs)
    coeffs = [round(i, sigdigits=4) for i in coeffs]
    n = length(coeffs)
    res = ""
    for (idx, i) in enumerate(coeffs)
        power = n - idx
        if i == 0
            continue
        end
        sign_str = i < 0 ? "-" : (isempty(res) ? "" : " + ")
        val = abs(i)
        if power > 0
            term = "$(val)z^{-$power}"
        else
            term = "$(val)"
        end
        res *= sign_str * term
    end
    return isempty(res) ? "0" : res
end

open("lua/1/3/rootsZ.lua", "w") do io
    println(io, "return { poles = {$(join(format_pole.(ControlSystemsBase.poles(Hz)), ", "))}, ")
    println(io, "zeros = {$(join(format_pole.(tzero(Hz)), ", "))} }")
end

open("lua/1/3/disk.lua", "w") do io
    println(io, "return \"H(z) = \\\\frac{$(poly_to_string(b_coeffs))}{$(poly_to_string(a_coeffs))}\"")
end

open("lua/1/3/recur.lua", "w") do io
    println(io, "return \"y[n] = $(join(["$(round(i, sigdigits=4))x[n$(k - 1 == 0 ? "" : "-$(k-1)")]" for (k, i) in enumerate(b_coeffs)], " + ")) -\\\\\\\\- $(join(["$(round(-i, sigdigits=4))y[n-$k]" for (k, i) in enumerate(a_coeffs[1:end-1])], " + "))\"")
end







# ================================================================================================================



z_poles = ControlSystemsBase.poles(Hz)
z_zeros = tzero(Hz)
k_gain = b_coeffs[end] / a_coeffs[end]

real_poles = filter(p -> abs(imag(p)) < 1e-6, z_poles)
complex_poles = filter(p -> abs(imag(p)) >= 1e-6, z_poles)

z_zeros_pool = copy(z_zeros)

println("\n" * repeat("=", 50))
println(" РАЗБИЕНИЕ H(z) НА КАСКАД ЗВЕНЬЕВ С УЧЕТОМ НУЛЕЙ ")
println("\n" * repeat("=", 50))

biquad_sections = []
seen_poles = Set()
sec_idx = 1

function take_zeros_coeffs(pool, count)
    if length(pool) >= count
        z_taken = [popfirst!(pool) for _ in 1:count]
        if count == 2
            z1, z2 = z_taken[1], z_taken[2]
            return [1.0, real(-(z1 + z2)), real(z1 * z2)]
        elseif count == 1
            z1 = z_taken[1]
            return [1.0, real(-z1)]
        end
    end
    return count == 2 ? [1.0, 0.0, 0.0] : [1.0, 0.0]
end

for p in complex_poles
    if p in seen_poles || conj(p) in seen_poles; continue; end
    push!(seen_poles, p)
    push!(seen_poles, conj(p))

    a1 = -2 * real(p)
    a2 = abs2(p)

    b_coeffs_sec = take_zeros_coeffs(z_zeros_pool, 2)

    push!(biquad_sections, (b=b_coeffs_sec, a=[1.0, a1, a2]))

    println("Звено 2-го порядка №$sec_idx:")
    println("  Полюса:     $(round(p, sigdigits=4)) и $(round(conj(p), sigdigits=4))")
    println("  Числитель:  b = $(round.(b_coeffs_sec, sigdigits=4))")
    println("  Знаменатель: a = $(round.([1.0, a1, a2], sigdigits=4))\n")

    global sec_idx += 1
end

for p in real_poles
    a1 = -real(p)

    b_coeffs_sec = take_zeros_coeffs(z_zeros_pool, 1)

    push!(biquad_sections, (b=b_coeffs_sec, a=[1.0, a1]))

    println("Звено 1-го порядка:")
    println("  Полюс:      $(round(p, sigdigits=4))")
    println("  Числитель:  b = $(round.(b_coeffs_sec, sigdigits=4))")
    println("  Знаменатель: a = $(round.([1.0, a1], sigdigits=4))\n")
end

println("Общий коэффициент усиления каскада (k) = $(round(k_gain, sigdigits=4))")

open("lua/1/3/biquads.lua", "w") do io
    println(io, "return {")
    println(io, "    k_gain = $(round(k_gain, sigdigits=4)),")
    println(io, "    sections = {")
    for sec in biquad_sections
        b_str = join(sec.b, ", ")
        a_str = join(sec.a, ", ")
        println(io, "        { b = {$b_str}, a = {$a_str} },")
    end
    println(io, "    }")
    println(io, "}")
end

let
    N_steps = 100
    x = zeros(N_steps)
    x[1] = 1.0
    y_global = lsim(Hz, reshape(x, 1, :))[1][1, :]
    local current_signal = copy(x)
    for sec in biquad_sections
        b_sec = sec.b
        a_sec = sec.a
        out_sec = zeros(N_steps)
        for n in 1:N_steps
            s_in  = sum(k -> (n-k >= 1) ? b_sec[k+1] * current_signal[n-k] : 0.0, 0:(length(b_sec)-1))
            s_out = sum(k -> (n-k >= 1) ? a_sec[k+1] * out_sec[n-k] : 0.0, 1:(length(a_sec)-1))

            out_sec[n] = s_in - s_out
        end
        current_signal = out_sec
    end
    y_cascade = current_signal .* k_gain
    max_diff = maximum(abs.(y_global .- y_cascade))
    println("\n" * repeat("=", 50))
    println(" ПРОВЕРКА ЭКВИВАЛЕНТНОСТИ РЕКУРРЕНТНЫХ УРАВНЕНИЙ ")
    println(repeat("=", 50))
    println("Первые 5 отсчетов h[n] (глобальная система):  ", round.(y_global[1:5], sigdigits=4))
    println("Первые 5 отсчетов h[n] (каскад звеньев):       ", round.(y_cascade[1:5], sigdigits=4))
    println("Максимальная абсолютная погрешность:         $max_diff")
    if max_diff < 1e-7
        println("Вывод: Эквивалентность ПОЛНОСТЬЮ ДОКАЗАНА (погрешность на уровне машинного нуля).")
    else
        println("Вывод: Обнаружена расходимость.")
    end
end




