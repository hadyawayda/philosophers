# Algorithm Test cases

#### Philosopher 1 takes a fork and dies after 200 ms.

./philo 1 200 200 200

#### No philosopher dies.

./philo 5 800 200 200

#### The program stops when each philosopher has eaten 7 times.

./philo 5 800 200 200 7

#### No philosopher dies.

./philo 4 410 200 200

#### A philosopher dies.

./philo 4 310 200 200

#### A philosopher dies after 500 ms -

./philo 4 500 200 2147483647

#### No philosopher dies.

./philo 4 2147483647 200 200

#### A philosopher dies, it should display the death before 210 ms. -

./philo 4 200 210 200

#### No philosopher dies.

./philo 5 800 200 150

# Validation Test cases

#### Invalid argument.

./philo 4 500 200 1.2

#### Invalid argument.

./philo 4 0 200 200

#### Invalid argument.

./philo 4 -500 200 200

#### Invalid argument.

./philo 4 214748364732 200 200

# Frequency check

#### 1- Philosopher ate seven times in the “must-eat = 7” test.

./philo 5 800 200 200 7 > log5.txt

#### wait until the prompt comes back

./philo 5 800 200 200 7 > log5.txt
awk '$3=="is" && $4=="eating" {cnt[$2]++}
END{for(i=1;i<=5;i++) printf "philo %d ate %d times\n", i, cnt[i]+0}' \
 log5.txt

#### 2-Check the timestamp on that death line (≈ 500 ms) and that nothing is printed afterwards

./philo 4 500 200 2147483647 > log.txt
grep -n "died" log.txt

awk '
$3=="is" && $4=="eating" { last[$2] = $1 } # save last-eat timestamp
/died/ { # when we hit the death line…
id = $2
die = $1
gap = die - last[id]
print "philo", id, "last meal at", last[id] "ms;",
"died at", die "ms → Δ", gap, "ms"
}
END {
if (NR != FNR) { } # just silence awk lint warnings
}
' log.txt

# Data races and leaks check

```
valgrind --tool=helgrind ./philo 5 800 200 200
```

```
#4-valgrind --leak-check=full ./philo 5 800 200 200
```
