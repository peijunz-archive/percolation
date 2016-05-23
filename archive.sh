rm single.sh.e*
tar -Jcvf "data-$(date +%d-%H%M%S).tar.xz" data/
rm data/*.dat
