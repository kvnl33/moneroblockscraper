# CMake generated Testfile for 
# Source directory: /home/yorozuya/monero/tests/hash
# Build directory: /home/yorozuya/monero/build/release/tests/hash
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(hash-fast "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "fast" "/home/yorozuya/monero/tests/hash/tests-fast.txt")
add_test(hash-slow "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "slow" "/home/yorozuya/monero/tests/hash/tests-slow.txt")
add_test(hash-tree "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "tree" "/home/yorozuya/monero/tests/hash/tests-tree.txt")
add_test(hash-extra-blake "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "extra-blake" "/home/yorozuya/monero/tests/hash/tests-extra-blake.txt")
add_test(hash-extra-groestl "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "extra-groestl" "/home/yorozuya/monero/tests/hash/tests-extra-groestl.txt")
add_test(hash-extra-jh "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "extra-jh" "/home/yorozuya/monero/tests/hash/tests-extra-jh.txt")
add_test(hash-extra-skein "/home/yorozuya/monero/build/release/tests/hash/hash-tests" "extra-skein" "/home/yorozuya/monero/tests/hash/tests-extra-skein.txt")
