N = 64
# for i = 1:N
#     parameter_list = join( ["_$(j)" for j in 1:i],", ")
#     println("#define PP_FOR_EACH_$(i)(func, separator, $(parameter_list))\\")
#     println(join( ["func(_$(j))" for j in 1:i]," separator "))
# end

# println(join(reverse( ["$(j)" for j in 1:N]),", "))
println(join( ["_$(j)" for j in 1:N],", "))