make
make test

for D in $(seq 0 2); do
  cd checker
  g++ checker-${D}.cpp
  cd ..
  mv checker/a.out ./tests
  cp main ./tests
  cd tests
  cp debug${D}/sol* .
  cp debug${D}/out* .
  
  ./a.out
  rm main
  rm a.out
  rm sol*
  rm out*
  cd ..
done