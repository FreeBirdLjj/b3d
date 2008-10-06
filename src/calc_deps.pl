#!/usr/bin/env perl
while(<>) {
    if(/^#include\s+"/) {
        s/#include\s+"(.*)"/\1/;
        chop;
        print "src/$_" . ' ';
    }
}

