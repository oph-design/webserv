for i in {1..20000}
do
  curl localhost:1234 > /dev/null 2> /dev/null &
  echo $i
done