./run.sh
while fswatch -1 --event=Updated -r examples scripts include  ; do
    ./run.sh
    sleep 2
done