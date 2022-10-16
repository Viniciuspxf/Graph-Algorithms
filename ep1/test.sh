make main

for D in $(seq 0 2); do
  cd checkers
  g++ checker-${D}.cpp
  cd ..
  mv checkers/a.out ./tests
  cp main ./tests
  cd tests
  cp debug${D}/sol* .


  
  for test in $(seq 1 12); do
    formated_test=$(printf "%02d" ${test})
    cp in${formated_test} in${formated_test}.backup
    echo ${D} > in${formated_test}
    #add everything from old file starting from second line
    cat in${formated_test}.backup |tail -n+2>> in${formated_test}
    ./main < in${formated_test} > out${formated_test}
  done
  
  ./a.out
  rm main
  rm a.out
  rm sol*
  rm out*
  rm *.backup
  cd ..
done