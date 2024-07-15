N = 64
for i = 1:N
    parameter_list = join( ["_$(j)" for j in 1:i],", ")
#     __PP_FOR_EACH_1(func,brackets,separator,_1)
    print("#define __PP_FOR_EACH_$(i)(func, brackets, separator, $(parameter_list)) ")
#     func DEFER_PP_CONCAT(brackets,_BEGIN) _1 DEFER_PP_CONCAT(brackets,_END)
# PP_CONCAT(separator,_IMPL)
    print(join( ["func PP_CONCAT(brackets,_BEGIN) _$(j) PP_CONCAT(brackets,_END)" for j in 1:i]," PP_CONCAT(separator,_IMPL) "))
    println()
end

# println(join(reverse( ["$(j)" for j in 1:N]),", "))
# println(join( ["_$(j)" for j in 1:N],", "))