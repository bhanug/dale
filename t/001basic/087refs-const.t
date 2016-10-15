#!/usr/bin/env perl

use warnings;
use strict;
$ENV{"DALE_TEST_ARGS"} ||= "";
my $test_dir = $ENV{"DALE_TEST_DIR"} || ".";
$ENV{PATH} .= ":.";

use Data::Dumper;
use Test::More tests => 3;

my @res = `dalec $ENV{"DALE_TEST_ARGS"} $test_dir/t/src/refs-const.dt -o refs-const`;
is(@res, 0, 'No compilation errors');

@res = `./refs-const`;
is($?, 0, 'Program executed successfully');

chomp for @res;
is_deeply(\@res, [qw(100 500 0)], 'Got expected results');

`rm refs-const`;

1;
