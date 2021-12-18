out=$(basename -s .cpp $1)
g++ $1 -Wall -Wextra -Wcast-align -Wunused -Wpedantic -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wuseless-cast -Wundef -Wlogical-op -Wmissing-field-initializers -O3 -o $out
