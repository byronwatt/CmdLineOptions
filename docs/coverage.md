# coverage

followed the post: https://about.codecov.io/blog/how-to-set-up-codecov-with-c-and-github-actions/ by Aniket Bhattacharyea


meson setup coverage -Db_coverage=true

adding Db_coverage=true properly adds --coverage to the compile/link lines, but the codecov/codecov-action@v3 gcov:true doesn't find the files called ../src/xxx and ../example/xxx because of the '..' in the path.

I tried adding a 'fixes:' directive to remove the '../' but it didn't seem to help.

note: in https://codecov.io/gh/byronwatt/CmdLineOptions/branch/main -> 'browse reports', you can click on the 'view yaml' link to see what the yaml was:

i'm not sure if this comes from .codecov.yml or .github/workflows/codecov.yml, i should try adding a bogus 'fixes' option to see which files are used.

```yaml
codecov:
  require_ci_to_pass: true
comment:
  behavior: default
  layout: reach, diff, flags, files
  require_base: false
  require_changes: false
  require_head: false
  show_carryforward_flags: false
coverage:
  precision: 2
  range:
  - 70.0
  - 100.0
  round: down
  status:
    changes: false
    default_rules:
      flag_coverage_not_uploaded_behavior: include
    patch:
      default:
        informational: true
    project:
      default:
        target: auto
        threshold: 0.5
fixes:
- '[.][.]/::'
github_checks:
  annotations: true
```

## the final fix

I switched to cmake which can do in-place builds kinda,... but it turns out a cmake out-of-source build compiles with the full path to the source file, and the coverage script is fine with that.

I'd rather use meson to build, but I don't know how to change meson to build from absolute path names rather than relative path names.

## sample github workflow from Aniket Bhattacharyea

```yaml
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
```
