using Polynomials

mkpath("lua/1/1")
mkpath("lua/1/2")

function format_complex(arr)
    if isempty(arr)
        return "{\"\$\\\\emptyset\$\"}"
    end
    formatted = map(arr) do x
        re = round(real(x), digits=4)
        im = round(imag(x), digits=4)
        if im == 0
            return string(re)
            elseif re == 0
            return string(im) * "i"
        else
            sign_str = im > 0 ? "+" : "-"
            return string(re) * sign_str * string(abs(im)) * "i"
        end
    end
    return "{\"" * join(formatted, "\", \"") * "\"}"
end

function write_sys(sys, path, syst)
    open(path, "w") do io
        println(io, "return {\n")
        for (name, coeffs) in sys
            num_coeffs = coeffs[1]
            den_coeffs = coeffs[2]
            zeros_sys = length(num_coeffs) > 1 ? roots(Polynomial(num_coeffs)) : []
            poles_sys = length(den_coeffs) > 1 ? roots(Polynomial(den_coeffs)) : []
            if syst == "s"
                is_stable = isempty(poles_sys) ? true : all(real.(poles_sys) .< 0)
            elseif syst == "z"
                is_stable = isempty(poles_sys) ? true : all(abs.(poles_sys) .< 1)
            end
            println(io, "[\"$(name)\"] = { poles = $(format_complex(poles_sys)), zeros = $(format_complex(zeros_sys)), is_stable = $is_stable},\n")
        end
        println(io, "}")
    end
end

systems_s = Dict(
    "H_1" => [[1.0], [1.0]],
    "H_2" => [[1.0], [0.0, 0.5]],
    "H_3" => [[0.0, 0.5], [1.0]],
    "H_4" => [[0.0, 0.1], [1.0, 0.5]],
    "H_5" => [[1.0], [1.0, 0.5]],
)
for (n, a) in [1=>0.0, 2=>0.5, 3=>1.5]
    systems_s["H6_a$a"] = [[1.0], [1.0, a*0.5, 0.25]]
end

systems_z = Dict(
    "H_1" => [[1.0], [1.0]],
    "H_2" => [[0.0, 1.0], [-100.0, 100.0]],
    "H_3" => [[-100.0, 100.0], [0.0, 1.0]],
    "H_5" => [[100.0], [101.0, 1.0]],
    "H_6" => [[1.0], [1.0, 100.0, 10000]],
    "H_7" => [[1.0], [zeros(100); 1.0]],
)

write_sys(systems_s, "lua/1/1/sys.lua", "s")
write_sys(systems_z, "lua/1/2/sys.lua", "z")

function generate_recurrence(name, sys)
    num, den = sys
    a0 = den[1]
    if a0 != 0 && a0 != 1
        num = num ./ a0
        den = den ./ a0
    end
    rhs_terms = String[]
    for i in 1:length(num)
        if num[i] == 0; continue; end
        val = round(num[i], digits=4)
        term = (i-1 == 0) ? "x[n]" : "x[n-$(i-1)]"
        push!(rhs_terms, val == 1.0 ? term : "$(val)$term")
    end
    lhs_terms = String[]
    for j in 2:length(den)
        if den[j] == 0; continue; end
        val = round(-den[j], digits=4)
        term = "y[n-$(j-1)]"
        if val > 0
            push!(lhs_terms, val == 1.0 ? term : "$(val)$term")
            elseif val < 0
            push!(lhs_terms, val == -1.0 ? "- $term" : "- $(abs(val))$term")
        end
    end
    all_terms = vcat(rhs_terms, lhs_terms)
    return "[\"$name\"] = \"y[n] = $(isempty(all_terms) ? "0" : join(all_terms, " + "))\","
end

open("lua/1/rec.lua", "w") do io
    println(io, "return {")
    for (name, sys) in systems_z
        println(io, generate_recurrence(name,sys))
    end
    println(io, "}")
end
