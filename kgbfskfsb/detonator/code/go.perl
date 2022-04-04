#!/usr/bin/env perl 
use warnings;
use strict;
use IPC::Open2;

$| = 1;
chdir "/.do/you/see/the/answer/";

my $pid = open2(\*out2, \*in2, './runme') or die;

my $reply = <out2>;
$reply = substr $reply, 0, -4;
print STDOUT $reply; #captcha formula

my $answer = eval($reply);
print STDOUT "answer: $answer\n"; 

print in2 " $answer "; #send it to process
in2->flush();

$reply = <out2>;
print STDOUT $reply; #flag :D

