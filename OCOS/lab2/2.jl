using Polynomials
den = [1.0, 3.2361, 5.2361, 5.2361, 3.2361, 1.0]
open("lua/2lap.lua", "w") do io
    print(io, "return")
    for i in den
        print()

    end
end

poles = roots(Polynomial(den))
open("lua/2poles.lua", "w") do io println(io, "return { $(join(poles, ", ")) }") end
