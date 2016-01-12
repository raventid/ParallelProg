# Algorith based on 
# https://en.wikipedia.org/wiki/Numerical_differentiation 

# First argument is the function to be evaluated, as a Ruby lambda.
# Second argument is the value at which to evaluate the derivative.
# Returns a numeric approximation to f'(x), using machine epsilon
# scaling to avoid numerical instability issues that would occur if
# the h (delta-x) value is too close to zero.
#
def derivative(f = lambda {|value| value}, x)   # default f(x) = x
  h = x * Math.sqrt(Float::EPSILON)
  lower = x - h
  upper = x + h
  (f[upper] - f[lower]) / (upper - lower)
end


# General workflow
my_function = lambda {|x| 4.0 * x**2 + 5.0 * x}
value_of_interest = 10
derivative(my_function, value_of_interest)

# Extended example set
square = lambda {|value| value * value}    # or value**2 if you prefer
p derivative(square, 1.0)   # 2 * x = 2.0
p derivative(square, 2.0)   # 2 * x = 4.0
p derivative(square, 3.0)   # 2 * x = 6.0
square_root = lambda {|value| Math.sqrt(value)}
p derivative(square_root, 1.0)   # 1 / (2*sqrt(x)) = 0.5
p derivative(square_root, 2.0)   # 1 / (2*sqrt(x)) = 0.35355339059327373
p derivative(square_root, 3.0)   # 1 / (2*sqrt(x)) = 0.2886751345948129
cube = lambda {|value| value * value * value}   # or value**3 if you prefer
p derivative(cube, 1.0)   # 3 * x**2 = 3.0
p derivative(cube, 2.0)   # 3 * x**2 = 12.0
p derivative(cube, 3.0)   # 3 * x**2 = 27.0
