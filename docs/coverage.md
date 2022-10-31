# coverage

followed the post: https://about.codecov.io/blog/how-to-set-up-codecov-with-c-and-github-actions/ by Aniket Bhattacharyea


meson setup coverage -Db_coverage=true

adding Db_coverage=true properly adds --coverage to the compile/link lines, but the codecov/codecov-action@v3 gcov:true doesn't find the files called ../src/xxx and ../example/xxx because of the '..' in the path.

I tried adding a 'fixes:' directive to remove the '../' but it didn't seem to help.

so I switched to cmake which can do in-place builds kinda,... but it turns out a cmake out-of-source build compiles with the full path to the source file, and the coverage script is fine with that.

## sample github workflow

name: Workflow for Codecov example-c
on: [push, pull_request]
jobs:
  run:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v2
      - name: Compile calculator
        run: gcc -fprofile-arcs -ftest-coverage -O0 -o test_calculator test_calculator.c calculator.c
      - name: Run tests
        run: ./test_calculator
      - name: Upload coverage to Codecov
        uses: codecov/codecov-action@v3
        with:
          gcov: true
          gcov_include: calculator.c