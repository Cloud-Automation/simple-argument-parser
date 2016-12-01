echo "Executing unit tests..."
./build/tests/tests
result=$?
echo "Unit tests completed : $result"
exit $result
