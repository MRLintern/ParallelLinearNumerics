# Introduction
* This fould contains updated software which tests a range of `relaxation factors` between `[0, 2]`.
* The `Parallel Jacobi` and `Parallel Gauss-Seidel` methods are left unchanged.
* However, the files generated for these methods do mention a `relaxation factor` of `1.0`; this doesn't the methods.

## Results
* `CSV` files for each `relaxation factor` are generated.
* You will also find a `collated data set` which consists of all methods, along with the `convergence rates` corresponding to their `relaxation factor`.
* You will find a plot, `Parallel-SOR-Convergence-Rates-Various-RF.png`, which shows how many iterations were taken for a particular `relaxation factor`.
* The plot isn't great, however, what you can see is that with a `relaxation factor` of `2.0`, the `SOR algorithm` performs poorly; it took `10000 iterations to converge`!.
* For a `relaxation factor` of `1.0`, the algorithm performs very well. A `relaxation factor` of `1.0` is just the `Gauss-Seidel Method/Algorithm`.
