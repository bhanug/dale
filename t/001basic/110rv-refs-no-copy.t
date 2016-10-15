#!/usr/bin/env perl

use warnings;
use strict;
$ENV{"DALE_TEST_ARGS"} ||= "";
my $test_dir = $ENV{"DALE_TEST_DIR"} || ".";
$ENV{PATH} .= ":.";

use Data::Dumper;
use Test::More tests => 3;

my @res = `dalec $ENV{"DALE_TEST_ARGS"} $test_dir/t/src/rv-refs-no-copy.dt -o rv-refs-no-copy`;
is(@res, 0, 'No compilation errors');

@res = `./rv-refs-no-copy`;
is($?, 0, 'Program executed successfully');

chomp for @res;

is_deeply(\@res, [
'One setf-move-init',
'setf-move-init',
'One setf-move-assign',
'setf-move-assign',
'One setf-move-init',
'setf-move-init',
'Two setf-move-inits',
'setf-move-init',
'setf-move-init',
'Preswap',
'1 2',
'1 3',
'setf-move-init',
'setf-move-assign',
'setf-move-assign',
'Postswap',
'1 3',
'1 2',
], 'Got expected results');

`rm rv-refs-no-copy`;

1;
