# ok not just github problems, but github related,...

## creating a branch against an issue, submitting a pull request and commiting the change

very nice that it automatically closes the issue,... but how do you have it not automatically close the issue???

## code coverage.

running code coverage locally shows how often each line of code was executed, and show higher code coverage,... 

likely because if statements that cover both true and false cases show as completely coverage,... but the github code coverage tool shows it as not completely covered.
BUT IT IS THE SAME TOOL!  or at least it seems to be the same tool.

so why does the report at:

https://codecov.io/gh/byronwatt/CmdLineOptions/branch/main

looks so different than typing:
```bash
   make code_coverage
```

which compiles the code the same way and then runs:
```bash
	gcovr --html-details --output html/index.html
```

whereas the github action is:
```json
    - name: Upload coverage to Codecov
      uses: codecov/codecov-action@v3
      with:
        gcov: true
        token: ${{ secrets.CODECOV_TOKEN }}
```
which I assume is the same thing.

## codeql

still not working.

## showing test results as some sort of status using junit test results.

apparently there are lots of different junit test result formats, not sure what bats uses.  i guess pushing different changes to a branch might be able to test all those formats.

