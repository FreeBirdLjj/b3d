for i in *.c *.h ; do echo -n "src/$i: " ; ./calc_deps.pl $i ; echo ; done

