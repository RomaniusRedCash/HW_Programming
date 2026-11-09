using ControlSystemsBase
using Polynomials

mkpath("lua/1/3")

function foo(name, sys)
    Hs = tf(sys...)
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
                term = "$(val)*z^{-$power}"
            else
                term = "$(val)"
            end
            res *= sign_str * term
        end
        return isempty(res) ? "0" : res
    end

    open("lua/1/3/sys_$name.lua", "w") do io
        println(io, "return \"\$$name(z) = \\\\div{$(poly_to_string(b_coeffs))}{$(poly_to_string(a_coeffs))}\$\"")
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

for (name,sys) in systems_s
    foo(name,sys)
end
